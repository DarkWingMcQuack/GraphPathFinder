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
    template<class F>
    [[nodiscard]] auto sortEdgesAccordingTo(F&& func) noexcept
	    -> std::vector<common::EdgeID>
	requires std::regular_invocable<F, const Graph&>
	&& std::strict_weak_order<std::invoke_result_t<F, const Graph&>,
				        		  common::EdgeID,
								  common::EdgeID>
    // clang-format on
    {
        auto &graph = impl();
        const auto order = std::invoke(std::forward<F>(func), graph);

        std::vector permutation(graph.numberOfEdges(),
                                common::EdgeID{0});
        std::iota(std::begin(permutation),
                  std::end(permutation),
                  common::EdgeID{0});

        //sort the edgeids according to the given order
        std::sort(std::begin(permutation),
                  std::end(permutation),
                  order);

        //apply the permutation to the forward connections
        if constexpr(concepts::ForwardGraph<Graph>) {
            for(size_t i = 0; i < graph.numberOfNodes(); i++) {
                common::NodeID n{i};
                auto edge_ids = graph.getForwardEdgeIDsOf(n);
                for(std::size_t j = 0; j < edge_ids.size(); j++) {
                    edge_ids[j] = permutation[edge_ids[j]];
                }
            }
        }

        //apply the permutation to the backward connections
        if constexpr(concepts::BackwardGraph<Graph>) {
            for(size_t i = 0; i < graph.numberOfNodes(); i++) {
                common::NodeID n{i};
                auto edge_ids = graph.getBackwardEdgeIDsOf(n);
                for(std::size_t j = 0; j < edge_ids.size(); j++) {
                    edge_ids[j] = permutation[edge_ids[j]];
                }
            }
        }

        //copy the permutation here because it gets shuffled by the algorithm
        auto permutation_copy = permutation;

        //apply the permutation to the edges
        for(std::size_t i = 0; i < graph.numberOfEdges(); i++) {
            std::size_t curr = i;
            std::size_t next = permutation[curr];
            while(next != i) {
                swap(edges_[curr], edges_[next]);
                permutation[curr] = curr;
                curr = next;
                next = permutation[next];
            }
            permutation[curr] = curr;
        }

        return permutation_copy;
    }

    // clang-format off
private:
    //crtp helper function
    constexpr auto impl() noexcept
        -> Graph &
    {
        return static_cast<Graph &>(*this);
    }

    std::vector<Edge> edges_;
    // clang-format on
};

} // namespace graphs
