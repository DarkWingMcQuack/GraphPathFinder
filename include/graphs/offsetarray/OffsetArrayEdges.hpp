#pragma once

#include <common/BackwardEdgeView.hpp>
#include <concepts/BackwardGraph.hpp>
#include <concepts/EdgeWeights.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <numeric>
#include <vector>

namespace graphs {

template<class Graph, class Edge>
class OffsetArrayEdges
{
public:
    using EdgeType = Edge;

    OffsetArrayEdges(std::vector<Edge> edges) noexcept
        : edges_(std::move(edges))
    {

        // clang-format off
        static_assert(concepts::HasEdges<OffsetArrayEdges<Graph, Edge>>);
        // Edges have weights -> Offsetarrayedges has writable edgeweights
        static_assert(!concepts::HasWeight<Edge>
					  || concepts::WriteableEdgeWeights<OffsetArrayEdges<Graph, Edge>>);
        // clang-format on
    }

    OffsetArrayEdges(OffsetArrayEdges<Graph, Edge> &&) noexcept = default;
    OffsetArrayEdges(const OffsetArrayEdges<Graph, Edge> &) noexcept = default;

    constexpr auto edgeExists(common::EdgeID id) const noexcept -> bool
    {
        return id.get() < numberOfEdges();
    }

    constexpr auto getEdge(common::EdgeID id) const noexcept
        -> const Edge *
    {
        if(edgeExists(id)) {
            return &edges_[id.get()];
        }
        return nullptr;
    }

    constexpr auto getBackwardEdge(common::EdgeID id) const noexcept
        -> common::BackwardEdgeView<Edge>
    requires concepts::HasSource<Edge> && concepts::HasTarget<Edge>
    {
        const auto *edge_ptr = getEdge(id);
        return common::BackwardEdgeView{edge_ptr};
    }

    constexpr auto numberOfEdges() const noexcept -> std::size_t
    {
        return edges_.size();
    }

    // clang-format off
    constexpr auto getEdgeWeight(common::EdgeID id) const noexcept
        -> std::optional<common::Weight>
	    requires concepts::HasWeight<EdgeType>
    // clang-format on
    {
        if(const auto *edge = getEdge(id)) {
            return edge->getWeight();
        }
        return std::nullopt;
    }

    // clang-format off
    constexpr auto getEdgeWeightUnsafe(common::EdgeID id) const noexcept
        -> common::Weight
	    requires concepts::HasWeight<EdgeType>
    // clang-format on
    {
        return getEdge(id)->getWeight();
    }

    // clang-format off
    constexpr auto setEdgeWeight(common::EdgeID id, common::Weight weight) const noexcept
        -> void
	    requires concepts::HasWeight<EdgeType>
    // clang-format on
    {
        if(const auto *edge = getEdge(id)) {
            return edge->setWeight(weight);
        }
    }


    // clang-format off
private:
    friend Graph;

    std::vector<Edge> edges_;
    // clang-format on
};

} // namespace graphs
