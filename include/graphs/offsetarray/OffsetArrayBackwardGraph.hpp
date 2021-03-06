#pragma once

#include <common/BackwardEdgeView.hpp>
#include <common/Range.hpp>
#include <concepts/BackwardConnections.hpp>
#include <concepts/Edges.hpp>
#include <execution>
#include <vector>


// forward declare PHAST to be able to make it a friend of OffsetArrayBackwardGraph
namespace algorithms::distoracle {
template<class N, class E>
class PHAST;
}


namespace graphs {

template<class Node, class Edge, class Graph>
requires concepts::HasTarget<Edge> && concepts::HasSource<Edge>
class OffsetArrayBackwardGraph
{

public:
    using NodeType = Node;
    using EdgeType = Edge;

    OffsetArrayBackwardGraph() noexcept
    {
        static_assert(concepts::BackwardConnections<OffsetArrayBackwardGraph>);
        static_assert(concepts::SortableBackwardConnections<OffsetArrayBackwardGraph>);
        static_assert(concepts::DeletableBackwardConnections<OffsetArrayBackwardGraph>);

        const auto number_of_edges = impl().numberOfEdges();
        const auto number_of_nodes = impl().numberOfNodes();


        // build an adjacency list which then can be converted to an offset array
        std::vector<std::vector<common::EdgeID>> adjacency_list(number_of_nodes);
        for(std::size_t i = 0; i < number_of_edges; i++) {
            const auto *e = impl().getEdge(common::EdgeID{i});
            const auto trg = e->getTrg();
            adjacency_list[trg.get()].emplace_back(i);
        }

        // resize the offset array
        backward_offset_.resize(number_of_nodes + 1, 0);

        // reserve space for the flattened adjacency list
        backward_neigbours_.reserve(number_of_edges);

        // build the offset array
        for(std::size_t n = 0; n < number_of_nodes; n++) {
            const auto &neigs = adjacency_list[n];

            backward_neigbours_.insert(std::end(backward_neigbours_),
                                       std::begin(neigs),
                                       std::end(neigs));

            backward_offset_[n + 1] = backward_neigbours_.size();
        }
    }

    constexpr OffsetArrayBackwardGraph(OffsetArrayBackwardGraph &&) noexcept = default;
    constexpr OffsetArrayBackwardGraph(const OffsetArrayBackwardGraph &) noexcept = default;

    constexpr auto operator=(OffsetArrayBackwardGraph &&) noexcept
        -> OffsetArrayBackwardGraph & = default;
    constexpr auto operator=(const OffsetArrayBackwardGraph &) noexcept
        -> OffsetArrayBackwardGraph & = default;

    constexpr auto getBackwardEdgeIDBetween(common::NodeID from,
                                            common::NodeID to) const noexcept
        -> std::optional<common::EdgeID>
    {
        auto edge_ids = getBackwardEdgeIDsOf(from);
        auto iter = std::find_if(std::begin(edge_ids),
                                 std::end(edge_ids),
                                 [&](auto id) {
                                     return impl().getBackwardEdge(id)->getTrg() == to;
                                 });

        if(iter != std::end(edge_ids)) {
            return *iter;
        }

        return std::nullopt;
    }

    constexpr auto checkIfEdgeExistsBackwardBetween(common::NodeID from,
                                                    common::NodeID to) const noexcept
        -> bool
    {
        return getBackwardEdgeIDBetween(from, to).has_value();
    }


    constexpr auto getBackwardEdgeBetween(common::NodeID from,
                                          common::NodeID to) const noexcept
        -> common::BackwardEdgeView<EdgeType>
    {
        if(auto id_opt = getBackwardEdgeIDBetween(from, to)) {
            const auto *edge = impl().getEdge(id_opt.value());

            return common::BackwardEdgeView<EdgeType>(edge);
        }

        // TODO: think about a way to avoid this
        return common::BackwardEdgeView<EdgeType>(nullptr);
    }

    constexpr auto getBackwardEdgeIDsOf(common::NodeID node) const noexcept
        -> std::span<const common::EdgeID>
    {
        // if the node does not exist, return an empty span
        if(!impl().nodeExists(node)) {
            return {};
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
        if(!impl().nodeExists(node)) {
            return {};
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
	    requires std::regular_invocable<F, const Graph&>
	          && std::strict_weak_order<std::invoke_result_t<F, const Graph&>,
				         				common::EdgeID,
						        		common::EdgeID>
    // clang-format on
    {
        const auto order = std::invoke(std::forward<F>(func), impl());
        const auto range = common::range(impl().numberOfNodes());

        std::for_each(std::execution::par,
                      std::begin(range),
                      std::end(range),
                      [&](const auto i) {
                          auto ids = getBackwardEdgeIDsOf(common::NodeID{i});
                          std::sort(std::begin(ids), std::end(ids), order);
                      });
    }

    // clang-format off
    template<class F>
    auto deleteBackwardEdgesIDsIf(F&& func) noexcept
	    -> void
	    requires std::regular_invocable<F, const Graph&>
	          && std::predicate<std::invoke_result_t<F, const Graph&>,
				        		common::EdgeID>
    // clang-format on
    {
        const auto predicate = std::invoke(std::forward<F>(func), impl());
        const auto number_of_nodes = impl().numberOfNodes();
        const auto number_of_edges = impl().numberOfEdges();

        std::vector<size_t> backward_offset(number_of_nodes + 1, 0);
        std::vector<common::EdgeID> backward_neigbours;
        backward_neigbours.reserve(number_of_edges);

        for(size_t i = 0; i < number_of_nodes; i++) {
            auto ids = getBackwardEdgeIDsOf(common::NodeID{i});

            std::copy_if(std::begin(ids),
                         std::end(ids),
                         std::back_inserter(backward_neigbours),
                         [&](auto id) {
                             return !predicate(id);
                         });

            backward_offset[i + 1] = backward_neigbours.size();
        }
        this->backward_offset_ = std::move(backward_offset);
        this->backward_neigbours_ = std::move(backward_neigbours);
    }


    // clang-format off
private:

    //crtp helper function
    constexpr auto impl() const noexcept
	  -> const Graph &
    {
	  return static_cast<const Graph &>(*this);
    }

    friend Graph;
    //PHAST needs access to the backward_offset_
    friend class algorithms::distoracle::PHAST<Node, Edge>;

    std::vector<common::EdgeID> backward_neigbours_;
    std::vector<size_t> backward_offset_;
    // clang-format on
};

} // namespace graphs
