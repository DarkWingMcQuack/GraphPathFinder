#pragma once

#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <vector>

namespace graphs {


template<class Node, class Edge, class Graph>
class OffsetArrayForwardGraph
{
public:
    using NodeType = Node;
    using EdgeType = Edge;

    OffsetArrayForwardGraph() noexcept
        requires concepts::HasSource<EdgeType> && concepts::HasTarget<EdgeType>
    {
        static_assert(concepts::ForwardGraph<OffsetArrayForwardGraph<Node, Edge, Graph>>);

        const auto &nodes = impl().getNodes();
        const auto &edges = impl().getEdges();

        //build an adjacency list which then can be converted to an offset array
        std::vector<std::vector<common::EdgeID>> adjacency_list(nodes.size());
        for(std::size_t i = 0; i < edges.size(); i++) {
            const auto &e = edges[i];
            const auto src = e.getSrc();
            adjacency_list[src.get()].emplace_back(i);
        }

        //resize the offset array
        forward_offset_.resize(nodes.size() + 1, 0);

        //reserve space for the flattened adjacency list
        forward_neigbours_.reserve(edges.size());

        //build the offset array
        for(std::size_t n = 0; n < nodes.size(); n++) {
            const auto &neigs = adjacency_list[n];

            forward_neigbours_.insert(std::end(forward_neigbours_),
                                      std::begin(neigs),
                                      std::end(neigs));

            forward_offset_[n + 1] = forward_neigbours_.size();
        }
    }

    OffsetArrayForwardGraph(OffsetArrayForwardGraph &&) noexcept = default;
    OffsetArrayForwardGraph(const OffsetArrayForwardGraph &) noexcept = default;

    constexpr auto checkIfEdgeExistsBetween(common::NodeID from,
                                            common::NodeID to) const noexcept
        -> bool
    {
        return getForwardEdgeIDBetween(from, to).has_value();
    }

    constexpr auto getForwardEdgeIDBetween(common::NodeID from,
                                           common::NodeID to) const noexcept
        -> std::optional<common::EdgeID>
    {
        const auto edge_ids = getForwardEdgeIDsOf(from);
        const auto iter = std::find_if(std::cbegin(edge_ids),
                                       std::cend(edge_ids),
                                       [&](auto id) {
                                           return impl().getEdge(id)->getTrg() == to;
                                       });

        if(iter != std::end(edge_ids)) {
            return *iter;
        }

        return std::nullopt;
    }

    constexpr auto getForwardEdgeBetween(common::NodeID from,
                                         common::NodeID to) const noexcept
        -> const EdgeType *
    {
        if(const auto id_opt = getForwardEdgeIDBetween(from, to)) {
            return impl().getEdge(id_opt.value());
        }

        return nullptr;
    }

    constexpr auto getForwardEdgeIDsOf(common::NodeID node) const noexcept
        -> std::span<const common::EdgeID>
    {
        //if the node does not exist, return an empty span
        if(!impl().nodeExists(node)) {
            return std::span<const common::EdgeID>();
        }

        const auto start_offset = forward_offset_[node.get()];
        const auto end_offset = forward_offset_[node.get() + 1];
        const auto *start = &forward_neigbours_[start_offset];
        const auto *end = &forward_neigbours_[end_offset];

        return std::span{start, end};
    }

    constexpr auto getForwardEdgeIDsOf(common::NodeID node) noexcept
        -> std::span<common::EdgeID>
    {
        if(!impl().nodeExists(node)) {
            return std::span<common::EdgeID>();
        }

        const auto start_offset = forward_offset_[node.get()];
        const auto end_offset = forward_offset_[node.get() + 1];
        auto *start = &forward_neigbours_[start_offset];
        const auto *end = &forward_neigbours_[end_offset];

        return std::span{start, end};
    }

    /**
     * this one is a bit tricky,
     * the passed orders needs to be a function of type:
     * `OffsetArrayForwardGraph<Node, Edge> -> (NodeID, NodeID) -> bool`
     * this means it is callable with *this and returns a function
     * `(NodeID, NodeID) -> bool`
     * this function will then be used to sort the ids of the forward edges of all
     * nodes
     */
    // clang-format off
    template<class F>
    constexpr auto sortForwardEdgeIDsAccordingTo(F&& func) noexcept
        -> void
	    requires std::regular_invocable<F, decltype(*this)>
	          && std::strict_weak_order<decltype(func(*this)),
				        			    common::NodeID,
										common::NodeID>
    // clang-format on
    {
        const auto order = std::invoke(std::forward<F>(func), *this);

        for(std::size_t n = 0; n < impl().numberOfNodes(); n++) {
            auto ids = getForwardEdgeIDsOf(n);
            std::sort(std::begin(ids), std::end(ids), order);
        }
    }


private :
    // clang-format off

    //crtp helper function
    constexpr auto impl() const noexcept
        -> const Graph &
    {
        return static_cast<const Graph &>(*this);
    }

    std::vector<common::EdgeID> forward_neigbours_;
    std::vector<size_t> forward_offset_;
    // clang-format off
};

} // namespace graphs
