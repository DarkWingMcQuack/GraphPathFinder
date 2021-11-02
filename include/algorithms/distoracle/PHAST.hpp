#pragma once

#include <algorithms/distoracle/ch/CHDijkstraBackwardHelper.hpp>
#include <algorithms/distoracle/ch/CHDijkstraForwardHelper.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/DistanceOracle.hpp>
#include <fmt/core.h>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <numeric>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

template<class Node, class Edge>
class PHAST
{
public:
    constexpr static inline bool is_threadsafe = false;

    constexpr PHAST(const graphs::OffsetArray<Node, Edge>& graph) noexcept
        : graph_(graph),
          distances_(graph_.numberOfNodes(), common::INFINITY_WEIGHT)
    {
        static_assert(concepts::OneToManyDistanceOracle<PHAST>,
                      "PHAST should fullfill the OneToManyDistanceOracle concept");
    }

    [[nodiscard]] auto distancesFrom(common::NodeID src) noexcept
        -> const std::vector<common::Weight>&
    {
        if(last_src_.has_value() and last_src_.value() == src) {
            return distances_;
        }

        resetFor(src);
        upward(src);
        downward();
        return distances_;
    }

private:
    auto resetFor(common::NodeID src) noexcept
        -> void
    {
        for(auto& distance : distances_) {
            distance = common::INFINITY_WEIGHT;
        }

        last_src_ = src;
    }

    auto upward(common::NodeID src) noexcept
        -> void
    {
        pathfinding::DijkstraQueue heap;
        heap.emplace(src, 0);
        distances_[src.get()] = common::Weight{0};

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            const auto edge_ids = graph_.getForwardEdgeIDsOf(current_node);

            if(shouldStall(cost_to_current, edge_ids)) {
                continue;
            }


            for(const auto id : edge_ids) {
                const auto edge = graph_.getEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getWeight();

                const auto new_dist = cost + cost_to_current;

                if(new_dist < distances_[neig.get()]) {
                    heap.emplace(neig, new_dist);
                    distances_[neig.get()] = new_dist;
                }
            }
        }
    }

    constexpr auto shouldStall(common::Weight cost_to_current,
                               const std::span<const common::EdgeID>& edge_ids) const noexcept
        -> bool
    {
        for(const auto id : edge_ids) {
            const auto& edge = graph_.getEdge(id);
            const auto neig = edge->getTrg();
            const auto cost = edge->getWeight();

            const auto current_dist_to_neig = distances_[neig.get()];

            if(current_dist_to_neig == common::INFINITY_WEIGHT) {
                continue;
            }

            if(current_dist_to_neig + cost < cost_to_current) {
                return true;
            }
        }

        return false;
    }

    auto downward() noexcept
        -> void
    {
        for(const auto edge_id : graph_.backward_neigbours_) {
            const auto* edge = graph_.getEdge(edge_id);
            const auto src = edge->getSrc().get();
            const auto trg = edge->getTrg().get();
            const auto weight = edge->getWeight();

            // skip if src is not reachable from current node
            if(distances_[src] == common::INFINITY_WEIGHT) {
                continue;
            }

            distances_[trg] = std::min(distances_[trg],
                                       distances_[src] + weight);
        }
    }

private:
    const graphs::OffsetArray<Node, Edge>& graph_;
    std::vector<common::Weight> distances_;
    std::optional<common::NodeID> last_src_;
};


// clang-format off
template<class Node, class Edge>
[[nodiscard]] inline auto prepareGraphForPHAST(graphs::OffsetArray<Node, Edge> g) noexcept
    -> graphs::OffsetArray<Node, Edge>
// clang-format on
{
    g.deleteForwardEdgesIDsIf([](const auto& graph) {
        return [&](const auto id) {
            const auto* edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });


    g.deleteBackwardEdgesIDsIf([](const auto& graph) {
        return [&](const auto id) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });

    g.sortBackwardEdgeIDsAccordingTo([](const auto& graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_edge = graph.getBackwardEdge(lhs);
            const auto rhs_edge = graph.getBackwardEdge(rhs);
            const auto lhs_trg = lhs_edge->getTrg();
            const auto rhs_trg = rhs_edge->getTrg();
            const auto lhs_trg_lvl = graph.getNodeLevelUnsafe(lhs_trg);
            const auto rhs_trg_lvl = graph.getNodeLevelUnsafe(rhs_trg);
            return lhs_trg_lvl > rhs_trg_lvl;
        };
    });

    // TODO: maybe partition the edges such that all edges of
    // the backwardoffsetarray are alligned in memory
    g.sortEdgesAccordingTo([](const auto& graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_edge = graph.getBackwardEdge(lhs);
            const auto rhs_edge = graph.getBackwardEdge(rhs);
            const auto lhs_trg = lhs_edge->getTrg();
            const auto rhs_trg = rhs_edge->getTrg();
            const auto lhs_trg_lvl = graph.getNodeLevelUnsafe(lhs_trg);
            const auto rhs_trg_lvl = graph.getNodeLevelUnsafe(rhs_trg);

            return lhs_trg_lvl > rhs_trg_lvl;
        };
    });

    g.sortNodesAccordingTo([](const auto& graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_lvl = graph.getNodeLevelUnsafe(lhs);
            const auto rhs_lvl = graph.getNodeLevelUnsafe(rhs);
            return lhs_lvl > rhs_lvl;
        };
    });

    return g;
}

} // namespace algorithms::distoracle
