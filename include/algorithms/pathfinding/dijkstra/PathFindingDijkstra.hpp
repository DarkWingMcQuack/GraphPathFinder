#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstaQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <concepts/PathOracle.hpp>
#include <fmt/core.h>
#include <graphs/Path.hpp>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class Graph>
requires concepts::ForwardGraph<Graph> && concepts::HasEdges<Graph> && concepts::HasNodes<Graph>
class PathFindingDijkstra
{
public:
    constexpr static inline bool is_threadsafe = false;

    PathFindingDijkstra(const Graph& graph) noexcept
        : graph_(graph),
          distances_(graph.numberOfNodes(), common::INFINITY_WEIGHT),
          settled_(graph.numberOfNodes(), false),
          pq_(DijkstraQueueComparer{}),
          last_source_(std::nullopt),
          before_(graph.numberOfNodes(), common::UNKNOWN_NODE_ID)
    {
        static_assert(concepts::PathOracle<PathFindingDijkstra<Graph>>,
                      "PathFindingDijkstra should fullfill the PathFinding concept");

        static_assert(concepts::DistanceOracle<PathFindingDijkstra<Graph>>,
                      "PathFindingDijkstra should fullfill the PathFinding concept");
    }

    PathFindingDijkstra() = delete;
    PathFindingDijkstra(PathFindingDijkstra&&) noexcept = default;
    PathFindingDijkstra(const PathFindingDijkstra&) noexcept = default;
    auto operator=(const PathFindingDijkstra&) -> PathFindingDijkstra& = delete;
    auto operator=(PathFindingDijkstra&&) noexcept -> PathFindingDijkstra& = default;

    auto pathBetween(common::NodeID source, common::NodeID target) noexcept
        -> std::optional<graphs::Path>
    {
        //fill the distance array and calculate the cost
        auto cost = distanceBetween(source, target);
        if(cost == common::INFINITY_WEIGHT) {
            return std::nullopt;
        }

        //extract path starting from the target
        std::vector nodes{target};
        while(nodes.back() != source) {
            nodes.emplace_back(before_[nodes.back().get()]);
        }
        //reverse the vector to get the actual path
        std::reverse(std::begin(nodes), std::end(nodes));

        return graphs::Path(std::move(nodes),
                            cost);
    }

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

                if(common::INFINITY_WEIGHT != current_dist and neig_dist > new_dist) {
                    touched_.emplace_back(neig);
                    distances_[neig.get()] = new_dist;
                    pq_.emplace(neig, new_dist);
                    before_[neig.get()] = current_node;
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
            before_[n] = common::UNKNOWN_NODE_ID;
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
    std::vector<common::NodeID> before_;
};

} // namespace algorithms::pathfinding
