#pragma once

#include <common/BackwardEdgeView.hpp>
#include <concepts/EdgeWeights.hpp>
#include <concepts/Edges.hpp>
#include <graphs/Path.hpp>
#include <numeric>
#include <vector>

namespace graphs {

template<class Graph, class Edge>
class OffsetArrayEdges
{
public:
    using EdgeType = Edge;

    constexpr OffsetArrayEdges(std::vector<Edge> edges) noexcept
        : edges_(std::move(edges))
    {

        // clang-format off
        static_assert(concepts::HasEdges<OffsetArrayEdges<Graph, Edge>>);
        // Edges have weights -> Offsetarrayedges has writable edgeweights
        static_assert(!concepts::HasWeight<Edge>
					  || concepts::WriteableEdgeWeights<OffsetArrayEdges<Graph, Edge>>);
        // clang-format on
    }

    constexpr OffsetArrayEdges(OffsetArrayEdges &&) noexcept = default;
    constexpr OffsetArrayEdges(const OffsetArrayEdges &) noexcept = default;

    constexpr auto operator=(OffsetArrayEdges &&) noexcept
        -> OffsetArrayEdges & = default;
    constexpr auto operator=(const OffsetArrayEdges &) noexcept
        -> OffsetArrayEdges & = default;

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

    auto buildPathFromEdges(const std::vector<common::EdgeID> &edges) const noexcept
        -> std::optional<Path>
    requires concepts::HasSource<Edge> && concepts::HasTarget<Edge> && concepts::CanHaveShortcuts<Edge>
    {
        std::vector<common::NodeID> nodes;
        common::Weight weight{0};

        if(edges.empty()) {
            return Path{std::move(nodes), weight};
        }

        const auto first = getEdge(edges.front())->getSrc();
        nodes.emplace_back(first);

        for(const auto edge_id : edges) {

            std::vector edge_stack{edge_id};
            while(!edge_stack.empty()) {
                const auto current = edge_stack.back();
                edge_stack.pop_back();
                const auto *edge = getEdge(current);

                if(edge->isShortcut()) {
                    const auto [first, second] = edge->getShortcutUnsafe();
                    edge_stack.emplace_back(second);
                    edge_stack.emplace_back(first);
                    continue;
                }

                if(edge->getSrc() != nodes.back()) {
                    return std::nullopt;
                }

                nodes.emplace_back(edge->getTrg());

                weight += [&]() constexpr
                {
                    if constexpr(concepts::HasWeight<Edge>) {
                        return edge->getWeight();
                    }
                    return common::Weight{1};
                }
                ();
            }
        }

        return Path{std::move(nodes), weight};
    }


    auto buildPathFromEdges(const std::vector<common::EdgeID> &edges) const noexcept
        -> std::optional<Path>
    requires concepts::HasSource<Edge> && concepts::HasTarget<Edge> &&(!concepts::CanHaveShortcuts<Edge>)
    {
        std::vector<common::NodeID> nodes;
        common::Weight weight{0};

        if(edges.empty()) {
            return Path{std::move(nodes), weight};
        }

        const auto first = getEdge(edges.front())->getSrc();
        nodes.emplace_back(first);

        for(const auto edge_id : edges) {
            const auto *edge = getEdge(edge_id);

            if(edge->getSrc() != nodes.back()) {
                return std::nullopt;
            }

            nodes.emplace_back(edge->getTrg());
            weight += [&]() constexpr
            {
                if constexpr(concepts::HasWeight<Edge>) {
                    return edge->getWeight();
                }
                return common::Weight{1};
            }
            ();
        }

        return Path{std::move(nodes), weight};
    }


    // clang-format off
private:
    friend Graph;

    std::vector<Edge> edges_;
    // clang-format on
};

} // namespace graphs
