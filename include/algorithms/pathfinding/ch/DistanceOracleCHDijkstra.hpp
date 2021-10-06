#pragma once

#include <algorithms/pathfinding/ch/DistanceOracleCHDijkstraBackwardHelper.hpp>
#include <algorithms/pathfinding/ch/DistanceOracleCHDijkstraForwardHelper.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/BackwardEdges.hpp>
#include <concepts/BackwardGraph.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <concepts/NodeLevels.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class Graph, bool UseStallOnDemand = false>
// clang-format off
requires concepts::ForwardGraph<Graph>
      && concepts::BackwardGraph<Graph>
      && concepts::ReadableNodeLevels<Graph>
      && concepts::HasEdges<Graph>
      && concepts::HasBackwardEdges<Graph>
      && concepts::HasNodes<Graph>
      && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class DistanceOracleCHDijkstra : public DistanceOracleCHDijkstraForwardHelper<
                                     DistanceOracleCHDijkstra<Graph, UseStallOnDemand>,
                                     UseStallOnDemand>,
                                 public DistanceOracleCHDijkstraBackwardHelper<
                                     DistanceOracleCHDijkstra<Graph, UseStallOnDemand>,
                                     UseStallOnDemand>
{
    using ThisType = DistanceOracleCHDijkstra<Graph, UseStallOnDemand>;
    using ForwardHelper = DistanceOracleCHDijkstraForwardHelper<ThisType, UseStallOnDemand>;
    using BackwardHelper = DistanceOracleCHDijkstraBackwardHelper<ThisType, UseStallOnDemand>;

public:
    constexpr static inline bool is_threadsafe = false;

    DistanceOracleCHDijkstra(const Graph& graph) noexcept
        : ForwardHelper(graph.numberOfNodes()),
          BackwardHelper(graph.numberOfNodes()),
          graph_(graph) {}

    [[nodiscard]] auto distanceBetween(common::NodeID source, common::NodeID target) noexcept
        -> common::Weight
    {
        this->fillForwardInfo(source);
        this->fillBackwardInfo(target);

        auto top_node_opt = findShortestPathCommonNode();


        if(!top_node_opt) {
            return common::INFINITY_WEIGHT;
        }



        auto top_node = top_node_opt.value();

        return this->forward_distances_[top_node.get()]
            + this->backward_distances_[top_node.get()];
    }

    [[nodiscard]] auto findShortestPathCommonNode() const noexcept
        -> std::optional<common::NodeID>
    {
        if(this->forward_settled_.empty() or this->backward_settled_.empty()) {
            return std::nullopt;
        }

        auto best_node = this->forward_settled_[0];
        auto best_dist = common::INFINITY_WEIGHT;

        auto forward_idx = 0ul;
        auto backward_idx = 0ul;

        while(forward_idx < this->forward_settled_.size()
              and backward_idx < this->backward_settled_.size()) {

            if(this->forward_settled_[forward_idx] < this->backward_settled_[backward_idx]) {
                forward_idx++;
                continue;
            }

            if(this->forward_settled_[forward_idx] > this->backward_settled_[backward_idx]) {
                backward_idx++;
                continue;
            }

            const auto common = this->forward_settled_[forward_idx];
            const auto new_dist = this->forward_distances_[common.get()]
                + this->backward_distances_[common.get()];

            if(new_dist < best_dist) {
                best_dist = new_dist;
                best_node = common;
            }

            forward_idx++;
            backward_idx++;
        }

        if(best_dist.get() == common::INFINITY_WEIGHT.get()) {
            return std::nullopt;
        }

        return best_node;
    }

    [[nodiscard]] constexpr auto getGraph() const noexcept
        -> const Graph&
    {
        return graph_;
    }


private:
    const Graph& graph_;
};

} // namespace algorithms::pathfinding
