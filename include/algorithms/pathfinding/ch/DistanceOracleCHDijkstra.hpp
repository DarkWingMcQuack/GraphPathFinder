#pragma once

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

template<class Graph, bool UseStallOnDemand>
// clang-format off
requires concepts::ForwardGraph<Graph>
      && concepts::BackwardGraph<Graph>
      && concepts::ReadableNodeLevels<Graph>
      && concepts::HasEdges<Graph>
      && concepts::HasBackwardEdges<Graph>
      && concepts::HasNodes<Graph>
      && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class DistanceOracleCHDijkstra
{
public:
    constexpr static inline bool is_threadsafe = false;

    [[nodiscard]] auto distanceBetween(common::NodeID source, common::NodeID target) noexcept
        -> common::Weight
    {
        fillForwardInfo(source);
        fillBackwardInfo(target);

        auto top_node_opt = findShortestPathCommonNode();

        if(!top_node_opt) {
            return common::INFINITY_WEIGHT;
        }


        auto top_node = top_node_opt.value();

        return forward_distances_[top_node.get()]
            + backward_distances_[top_node.get()];
    }

    auto fillForwardInfo(common::NodeID source) noexcept
        -> void
    {
        if(last_source_ && last_source_.value().get() == source.get()) {
            return;
        }

        resetForwardFor(source);

        DijkstraQueue heap;
        heap.emplace(source, 0);

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            if(forward_already_settled_[current_node.get()]) {
                continue;
            }

            forward_already_settled_[current_node.get()] = true;

            const auto edge_ids = graph_.getForwardEdgeIDsOf(current_node);
            const auto current_level = graph_.getNodeLevel(current_node);

            if constexpr(UseStallOnDemand) {

                auto stall_on_demand_valid = false;
                for(const auto id : edge_ids) {
                    const auto& edge = graph_.getEdge(id);
                    const auto neig = edge.target;
                    const auto cost = edge.distance;

                    if(current_level >= graph_.getNodeLevel(neig)) {
                        break;
                    }

                    const auto current_dist_to_neig = forward_distances_[neig];

                    if(current_dist_to_neig == common::INFINITY_WEIGHT) {
                        continue;
                    }

                    if(current_dist_to_neig + cost < cost_to_current) {
                        stall_on_demand_valid = true;
                        break;
                    }
                }

                if(stall_on_demand_valid) {
                    continue;
                }
            }

            forward_settled_.emplace_back(current_node);

            for(const auto id : edge_ids) {
                const auto& edge = graph_.getEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getForwardWeight();
                const auto neig_level = graph_.getNodeLevel(neig);

                if(current_level >= neig_level) {
                    break;
                }

                const auto new_dist = cost + cost_to_current;

                if(new_dist < forward_distances_[neig]) {
                    heap.emplace(neig, new_dist);
                    forward_distances_[neig] = new_dist;
                    forward_touched_.emplace_back(neig);
                    forward_best_ingoing_[neig] = id;
                }
            }
        }

        std::sort(std::begin(forward_settled_),
                  std::end(forward_settled_));
    }

    auto fillBackwardInfo(common::NodeID target) noexcept
        -> void
    {
        if(last_target_ && last_target_.value().get() == target.get()) {
            return;
        }

        resetBackwardFor(target);

        DijkstraQueue heap;
        heap.emplace(target, 0);

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            if(backward_already_settled_[current_node.get()]) {
                continue;
            }

            const auto edge_ids = graph_.getBackwardEdgeIDsOf(current_node);
            const auto current_level = graph_.getNodeLevel(current_node);

            backward_already_settled_[current_node.get()] = true;

            if constexpr(UseStallOnDemand) {
                auto stall_on_demand_valid = false;
                for(const auto id : edge_ids) {
                    const auto& edge = graph_.getBackwardEdge(id);
                    const auto neig = edge->getTrg();
                    const auto cost = edge->getForwardWeight();

                    if(current_level >= graph_.getNodeLevel(neig)) {
                        break;
                    }

                    const auto current_dist_to_neig = backward_distances_[neig];

                    if(current_dist_to_neig == common::INFINITY_WEIGHT) {
                        continue;
                    }

                    if(current_dist_to_neig + cost < cost_to_current) {
                        stall_on_demand_valid = true;
                        break;
                    }
                }

                if(stall_on_demand_valid) {
                    continue;
                }
            }

            backward_settled_.emplace_back(current_node);

            for(const auto id : edge_ids) {
                const auto& edge = graph_.getEdge(id);
                const auto neig = edge.target;
                const auto neig_level = graph_.getNodeLevel(neig);

                if(current_level >= neig_level) {
                    break;
                }

                const auto new_dist = edge.distance + cost_to_current;

                if(new_dist < backward_distances_[neig]) {
                    heap.emplace(neig, new_dist);
                    backward_distances_[neig] = new_dist;
                    backward_touched_.emplace_back(neig);
                    backward_best_ingoing_[neig] = id;
                }
            }
        }

        std::sort(std::begin(backward_settled_),
                  std::end(backward_settled_));
    }

    [[nodiscard]] auto findShortestPathCommonNode() const noexcept
        -> std::optional<common::NodeID>
    {
        if(forward_settled_.empty() or backward_settled_.empty()) {
            return std::nullopt;
        }

        auto best_node = forward_settled_[0];
        auto best_dist = common::INFINITY_WEIGHT;

        auto forward_idx = 0ul;
        auto backward_idx = 0ul;

        while(forward_idx < forward_settled_.size()
              and backward_idx < backward_settled_.size()) {

            if(forward_settled_[forward_idx] < backward_settled_[backward_idx]) {
                forward_idx++;
                continue;
            }

            if(forward_settled_[forward_idx] > backward_settled_[backward_idx]) {
                backward_idx++;
                continue;
            }

            const auto common = forward_settled_[forward_idx];
            const auto new_dist = forward_distances_[common.get()] + backward_distances_[common.get()];

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

    auto resetForwardFor(common::NodeID node) noexcept
        -> void
    {
        for(const auto node : forward_touched_) {
            const auto n = node.get();
            forward_distances_[n] = common::INFINITY_WEIGHT;
            forward_best_ingoing_[n] = common::UNKNOWN_EDGE_ID;
            forward_already_settled_[n] = false;
        }
        forward_settled_.clear();
        forward_touched_.clear();

        last_source_ = node;
        forward_touched_.emplace_back(node);
        forward_distances_[node.get()] = common::Weight{0};
    }

    auto resetBackwardFor(common::NodeID node) noexcept
        -> void
    {
        for(const auto node : backward_touched_) {
            const auto n = node.get();
            backward_distances_[n] = common::INFINITY_WEIGHT;
            backward_best_ingoing_[n] = common::UNKNOWN_EDGE_ID;
            backward_already_settled_[n] = false;
        }
        backward_settled_.clear();
        backward_touched_.clear();

        last_target_ = node;
        backward_touched_.emplace_back(node);
        backward_distances_[node.get()] = common::Weight{0};
    }


private:
    const Graph& graph_;
    std::vector<common::Weight> forward_distances_;
    std::vector<common::NodeID> forward_settled_;
    std::vector<common::NodeID> forward_touched_;
    std::optional<common::NodeID> last_source_;
    std::vector<common::EdgeID> forward_best_ingoing_;
    std::vector<bool> forward_already_settled_;

    std::vector<common::Weight> backward_distances_;
    std::vector<common::NodeID> backward_settled_;
    std::vector<common::NodeID> backward_touched_;
    std::optional<common::NodeID> last_target_;
    std::vector<common::EdgeID> backward_best_ingoing_;
    std::vector<bool> backward_already_settled_;
};

} // namespace algorithms::pathfinding
