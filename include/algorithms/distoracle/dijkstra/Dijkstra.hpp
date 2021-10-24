#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

template<class Graph>
// clang-format off
requires concepts::ForwardConnections<Graph>
      && concepts::HasEdges<Graph>
      && concepts::HasNodes<Graph>
      && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class Dijkstra
{
public:
    constexpr static inline bool is_threadsafe = false;

    Dijkstra(const Graph& graph) noexcept
        : graph_(graph),
          distances_(graph.numberOfNodes(), common::INFINITY_WEIGHT),
          settled_(graph.numberOfNodes(), false),
          pq_(pathfinding::DijkstraQueueComparer{}),
          last_source_(std::nullopt)
    {
        static_assert(concepts::DistanceOracle<Dijkstra<Graph>>,
                      "Dijkstra should fullfill the DistanceOracle concept");
    }

    Dijkstra() = delete;
    Dijkstra(Dijkstra&&) noexcept = default;
    Dijkstra(const Dijkstra&) noexcept = default;
    auto operator=(const Dijkstra&) -> Dijkstra& = delete;
    auto operator=(Dijkstra&&) noexcept -> Dijkstra& = default;

    [[nodiscard]] auto distanceBetween(common::NodeID source, common::NodeID target) noexcept
        -> common::Weight
    {
        if(last_source_.has_value()
           and last_source_.value() == source
           and settled_[target.get()]) {
            return distances_[target.get()];
        }

        if(!last_source_.has_value() or source.get() != last_source_.value().get()) {
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

                //use the edge weight if available otherwise every edge has a weight 1
                const auto distance = [&]() constexpr
                {
                    if constexpr(concepts::HasWeight<typename Graph::EdgeType>) {
                        return edge->getWeight();
                    } else {
                        return common::Weight{1};
                    }
                }
                ();

                const auto neig_dist = distances_[neig.get()];
                const auto new_dist = current_dist + distance;

                if(common::INFINITY_WEIGHT.get() != current_dist.get() and neig_dist > new_dist) {
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
        pq_ = pathfinding::DijkstraQueue{pathfinding::DijkstraQueueComparer{}};

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
    pathfinding::DijkstraQueue pq_;
    std::optional<common::NodeID> last_source_;
};

} // namespace algorithms::distoracle
