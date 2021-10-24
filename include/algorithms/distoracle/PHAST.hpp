#pragma once

#include <algorithms/distoracle/ch/CHDijkstraBackwardHelper.hpp>
#include <algorithms/distoracle/ch/CHDijkstraForwardHelper.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/BackwardConnections.hpp>
#include <concepts/BackwardEdges.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <concepts/NodeLevels.hpp>
#include <fmt/core.h>
#include <numeric>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

template<class Graph>
// clang-format off
  requires concepts::ForwardConnections<Graph>
  && concepts::BackwardConnections<Graph>
  && concepts::ReadableNodeLevels<Graph>
  && concepts::HasEdges<Graph>
  && concepts::HasBackwardEdges<Graph>
  && concepts::HasNodes<Graph>
  && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class PHAST
{
public:
    constexpr static inline bool is_threadsafe = false;

    constexpr PHAST(const Graph& graph) noexcept
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

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            const auto edge_ids = graph_.getForwardEdgeIDsOf(current_node);
            const auto current_level = graph_.getNodeLevel(current_node).value_or(common::MAX_LEVEL);

            if(shouldStall(current_level, cost_to_current, edge_ids)) {
                continue;
            }


            for(const auto id : edge_ids) {
                const auto edge = graph_.getEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getWeight();
                const auto neig_level = graph_.getNodeLevel(neig);

                if(current_level >= neig_level) {
                    break;
                }

                const auto new_dist = cost + cost_to_current;

                if(new_dist < distances_[neig.get()]) {
                    heap.emplace(neig, new_dist);
                    distances_[neig.get()] = new_dist;
                }
            }
        }
    }

    constexpr auto shouldStall(common::NodeLevel current_level,
                               common::Weight cost_to_current,
                               const std::span<const common::EdgeID>& edge_ids) const noexcept
        -> bool
    {
        auto stall_on_demand_valid = false;
        for(const auto id : edge_ids) {
            const auto& edge = graph_.getEdge(id);
            const auto neig = edge->getTrg();
            const auto cost = edge->getWeight();

            if(current_level >= graph_.getNodeLevel(neig)) {
                return stall_on_demand_valid;
            }

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
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto weight = edge->getWeight();

            distances_[trg] = std::min(distances_[trg],
                                       distances_[src] + weight);
        }
    }



private:
    const Graph& graph_;
    std::vector<common::Weight> distances_;
    std::optional<common::NodeID> last_src_;
};


} // namespace algorithms::distoracle
