#pragma once

#include <common/BackwardEdgeView.hpp>
#include <concepts/BackwardGraph.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <concepts/NodeLevels.hpp>
#include <graphs/offsetarray/OffsetArrayEdges.hpp>
#include <graphs/offsetarray/OffsetArrayNodes.hpp>
#include <vector>

namespace graphs {


template<class Node, class Edge>
// the edges need to be able to access the target they are pointing to
// as well as the source from which they are starting from,
// this is needed to build Backwardedgeviews from Edges which inverts source and targets
// to get a backward edge
requires concepts::HasTarget<Edge> && concepts::HasSource<Edge>
class OffsetArrayBackwardGraph : public OffsetArrayNodes<Node>,
                                 public OffsetArrayEdges<Edge>
{
    static_assert(concepts::BackwardGraph<OffsetArrayBackwardGraph<Node, Edge>>);
    static_assert(concepts::HasNodes<OffsetArrayBackwardGraph<Node, Edge>>);
    static_assert(concepts::HasEdges<OffsetArrayBackwardGraph<Node, Edge>>);

    // clang-format off
    static_assert(!concepts::HasWeight<Edge>
				  || concepts::WriteableEdgeWeights<OffsetArrayBackwardGraph<Node, Edge>>);
    static_assert(!concepts::HasLevel<Node>
				  || concepts::WriteableNodeLevels<OffsetArrayBackwardGraph<Node, Edge>>);
    // clang-format on

public:
    OffsetArrayBackwardGraph(const std::vector<Node> &nodes,
                             const std::vector<Edge> &edges) noexcept
    {
        //build an adjacency list which then can be converted to an offset array
        std::vector<std::vector<common::EdgeID>> adjacency_list(nodes.size());
        for(std::size_t i = 0; i < edges.size(); i++) {
            const auto &e = edges[i];
            const auto trg = e.getTrg();
            adjacency_list[trg].emplace_back(i);
        }

        //resize the offset array
        backward_offset_.resize(nodes.size() + 1, 0);

        //reserve space for the flattened adjacency list
        backward_neigbours_.reserve(edges.size());

        //build the offset array
        for(std::size_t n = 0; n < nodes.size(); n++) {
            const auto &neigs = adjacency_list[n];

            backward_neigbours_.insert(std::end(backward_neigbours_),
                                       std::begin(neigs),
                                       std::end(neigs));

            backward_offset_[n + 1] = backward_neigbours_.size();
        }
    }

    constexpr auto checkIfEdgeExistsBetween(common::NodeID from,
                                            common::NodeID to) const noexcept
        -> bool
    {
        return getBackwardEdgeIDBetween(from, to).hasValue();
    }

    constexpr auto getBackwardEdgeIDBetween(common::NodeID from,
                                            common::NodeID to) const noexcept
        -> std::optional<common::EdgeID>
    {
        auto edge_ids = getBackwardEdgeIDsOf(from);
        auto iter = std::find(std::begin(edge_ids), std::end(edge_ids),
                              [&](auto id) { return getEdge(id).getTrg() == to; });

        if(iter != std::end(edge_ids)) {
            return *iter;
        }

        return std::nullopt;
    }

    constexpr auto getBackwardEdgeBetween(common::NodeID from,
                                          common::NodeID to) const noexcept
        -> common::BackwardEdgeView<Edge>
    {
        if(auto id_opt = getBackwardEdgeIDBetween(from, to)) {
            const auto *edge = getEdge(id_opt.value());

            return common::BackwardEdgeView<Edge>(edge);
        }

        //TODO: think about a way to avoid this
        return common::BackwardEdgeView<Edge>(nullptr);
    }

    constexpr auto getBackwardEdgeIDsOf(common::NodeID node) const noexcept
        -> std::span<const common::EdgeID>
    {
        //if the node does not exist, return an empty span
        if(!this->nodeExists(node)) {
            return std::span<const common::EdgeID>();
        }

        const auto start_offset = backward_offset_[node.get()];
        const auto end_offset = backward_offset_[node.get() + 1];
        const auto *start = &backward_neigbours_[start_offset];
        const auto *end = &backward_neigbours_[end_offset];

        return std::span{start, end};
    }

    constexpr auto getBackwardEdgeIDsOf(common::NodeID node) noexcept
        -> std::span<common::EdgeID>
    {
        if(!this->nodeExists(node)) {
            return std::span<common::EdgeID>();
        }

        const auto start_offset = backward_offset_[node.get()];
        const auto end_offset = backward_offset_[node.get() + 1];
        auto *start = &backward_neigbours_[start_offset];
        const auto *end = &backward_neigbours_[end_offset];

        return std::span{start, end};
    }

    /**
     * this one is a bit tricky,
     * the passed orders needs to be a function of type:
     * `OffsetArrayBackwardGraph<Node, Edge> -> (NodeID, NodeID) -> bool`
     * this means it is callable with *this and returns a function
     * `(NodeID, NodeID) -> bool`
     * this function will then be used to sort the ids of the backward edges of all
     * nodes
     */
    // clang-format off
    template<class F>
    constexpr auto sortBackwardEdgeIDsAccordingTo(F&& func) noexcept
        -> void
	    requires std::regular_invocable<F, decltype(*this)>
	          && std::strict_weak_order<decltype(func(*this)),
				        			    common::NodeID,
										common::NodeID>
    // clang-format on
    {
        auto order = std::invoke(std::forward<F>(func), *this);

        for(std::size_t n = 0; n < this->numberOfNodes(); n++) {
            auto ids = getBackwardEdgeIDsOf(n);
            std::sort(std::begin(ids), std::end(ids), order);
        }
    }


    // clang-format off
private:
    std::vector<common::EdgeID> backward_neigbours_;
    std::vector<size_t> backward_offset_;
    // clang-format on
};

} // namespace graphs
