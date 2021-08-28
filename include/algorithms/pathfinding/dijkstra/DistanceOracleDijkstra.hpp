#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstaQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class Graph>
requires concepts::ForwardGraph<Graph> && concepts::HasEdges<Graph>
class DistanceOracleDijkstra
{
public:
    constexpr static inline bool is_threadsafe = false;

    DistanceOracleDijkstra(const Graph& graph) noexcept
        : graph_(graph),
          distances_(graph.numberOfNodes(), common::INFINITY_WEIGHT),
          settled_(graph.numberOfNodes(), false),
          pq_(DijkstraQueueComparer{}),
          last_source_(std::nullopt)
    {
        static_assert(concepts::DistanceOracle<DistanceOracleDijkstra<Graph>>,
                      "DistanceOracleDijkstra should fullfill the DistanceOracle concept");
    }

    DistanceOracleDijkstra() = delete;
    DistanceOracleDijkstra(DistanceOracleDijkstra&&) noexcept = default;
    DistanceOracleDijkstra(const DistanceOracleDijkstra&) noexcept = default;
    auto operator=(const DistanceOracleDijkstra&) -> DistanceOracleDijkstra& = delete;
    auto operator=(DistanceOracleDijkstra&&) noexcept -> DistanceOracleDijkstra& = default;

    auto distanceBetween(common::NodeID source, common::NodeID target) noexcept
        -> common::Weight
    {
        if(last_source_.has_value()
           and last_source_.value() == source
           and settled_[target.get()]) {
            return distances_[target.get()];
        }

        if(!last_source_.has_value() or source != last_source_.value()) {
            resetFor(source);
        }

        while(!pq_.empty()) {
            const auto [current_node, current_dist] = pq_.top();

            settled_[current_node.get()] = true;

            if(current_node == target) {
                return current_dist;
            }

            //pop after the return, otherwise we loose a value
            //when reusing the pq
            pq_.pop();

            const auto edge_ids = graph_.getForwardEdgeIDsOf(current_node);

            for(auto id : edge_ids) {
                const auto* edge = graph_.getEdge(id);
                const auto neig = edge->getTrg();
                const auto distance = edge->getWeight();
                const auto neig_dist = distances_[neig.get()];
                const auto new_dist = current_dist + distance;

                if(common::INFINITY_WEIGHT != current_dist and neig_dist > new_dist) {
                    touched_.emplace_back(neig);
                    distances_[neig.get()] = new_dist;
                    pq_.emplace(neig, new_dist);
                }
            }
        }

        return common::INFINITY_WEIGHT;
    }

private:
    constexpr auto resetFor(common::NodeID new_source) noexcept
        -> void
    {
        for(const auto id : touched_) {
            const auto n = id.get();
            settled_[n] = false;
            distances_[n] = common::INFINITY_WEIGHT;
        }

        touched_.clear();
        pq_ = DijkstraQueue{DijkstraQueueComparer{}};

        last_source_ = new_source;
        pq_.emplace(new_source, 0l);
        distances_[new_source.get()] = common::Weight{0};
        touched_.emplace_back(new_source);
    }

private:
    const Graph& graph_;
    std::vector<common::Weight> distances_;
    std::vector<bool> settled_;
    std::vector<common::NodeID> touched_;
    DijkstraQueue pq_;
    std::optional<common::NodeID> last_source_;
};

} // namespace algorithms::pathfinding
