#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <optional>
#include <queue>
#include <span>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class CRTP, bool UseStallOnDemand>
class CHDijkstraForwardHelper
{
public:
    constexpr auto operator=(const CHDijkstraForwardHelper&) noexcept
        -> CHDijkstraForwardHelper& = delete;

    constexpr CHDijkstraForwardHelper(const CHDijkstraForwardHelper&) noexcept = delete;

private:
    constexpr CHDijkstraForwardHelper(std::size_t number_of_nodes)
        : forward_distances_(number_of_nodes, common::INFINITY_WEIGHT),
          forward_best_ingoing_(number_of_nodes, common::UNKNOWN_EDGE_ID),
          forward_already_settled_(number_of_nodes, false) {}

    constexpr CHDijkstraForwardHelper(CHDijkstraForwardHelper&&) noexcept = default;

    constexpr auto operator=(CHDijkstraForwardHelper&&) noexcept
        -> CHDijkstraForwardHelper& = default;

    auto fillForwardInfo(common::NodeID source) noexcept
        -> void
    {
        if(forward_last_source_ == source) {
            return;
        }

        resetForwardFor(source);

        pathfinding::DijkstraQueue heap;
        heap.emplace(source, 0);

        const auto& graph = getGraph();

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            if(forward_already_settled_[current_node.get()]) {
                continue;
            }

            forward_already_settled_[current_node.get()] = true;

            const auto edge_ids = graph.getForwardEdgeIDsOf(current_node);

            if constexpr(UseStallOnDemand) {
                if(shouldStall(cost_to_current, edge_ids)) {
                    continue;
                }
            }

            forward_settled_.emplace_back(current_node);

            for(const auto id : edge_ids) {
                const auto edge = graph.getEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getWeight();
                const auto new_dist = cost + cost_to_current;

                if(new_dist < forward_distances_[neig.get()]) {
                    heap.emplace(neig, new_dist);
                    forward_distances_[neig.get()] = new_dist;
                    forward_touched_.emplace_back(neig);
                    forward_best_ingoing_[neig.get()] = id;
                }
            }
        }

        std::sort(std::begin(forward_settled_),
                  std::end(forward_settled_));
    }

    constexpr auto shouldStall(common::Weight cost_to_current,
                               const std::span<const common::EdgeID>& edge_ids) const noexcept
        -> bool
    {
        for(const auto id : edge_ids) {
            const auto* edge = getGraph().getEdge(id);
            const auto neig = edge->getTrg();
            const auto cost = edge->getWeight();
            const auto current_dist_to_neig = forward_distances_[neig.get()];

            if(current_dist_to_neig == common::INFINITY_WEIGHT) {
                continue;
            }

            if(current_dist_to_neig + cost < cost_to_current) {
                return true;
            }
        }

        return false;
    }

    constexpr auto resetForwardFor(common::NodeID node) noexcept
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

        forward_last_source_ = node;
        forward_touched_.emplace_back(node);
        forward_distances_[node.get()] = common::Weight{0};
    }

    [[nodiscard]] constexpr auto getGraph() const noexcept
        -> decltype(auto)
    {
        return static_cast<const CRTP*>(this)->graph_;
    }

private:
    friend CRTP;
    std::vector<common::Weight> forward_distances_;
    std::vector<common::NodeID> forward_touched_;
    std::optional<common::NodeID> forward_last_source_;
    std::vector<common::EdgeID> forward_best_ingoing_;
    std::vector<bool> forward_already_settled_;
    std::vector<common::NodeID> forward_settled_;
};

} // namespace algorithms::distoracle
