#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <optional>
#include <queue>
#include <span>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

template<class CRTP, bool UseStallOnDemand>
class CHDijkstraBackwardHelper
{
public:
    // deleted things should be public
    constexpr CHDijkstraBackwardHelper(const CHDijkstraBackwardHelper&) noexcept = delete;
    constexpr auto operator=(const CHDijkstraBackwardHelper&) noexcept
        -> CHDijkstraBackwardHelper& = delete;

private:
    constexpr CHDijkstraBackwardHelper(std::size_t number_of_nodes)
        : backward_distances_(number_of_nodes, common::INFINITY_WEIGHT),
          backward_best_ingoing_(number_of_nodes, common::UNKNOWN_EDGE_ID),
          backward_already_settled_(number_of_nodes, false) {}
    constexpr CHDijkstraBackwardHelper(CHDijkstraBackwardHelper&&) noexcept = default;

    constexpr auto operator=(CHDijkstraBackwardHelper&&) noexcept
        -> CHDijkstraBackwardHelper& = default;

    auto fillBackwardInfo(common::NodeID source) noexcept
        -> void
    {
        if(last_source_ && last_source_.value().get() == source.get()) {
            return;
        }

        resetBackwardFor(source);

        pathfinding::DijkstraQueue heap;
        heap.emplace(source, 0);

        while(!heap.empty()) {
            const auto [current_node, cost_to_current] = heap.top();
            heap.pop();

            if(backward_already_settled_[current_node.get()]) {
                continue;
            }

            backward_already_settled_[current_node.get()] = true;

            const auto edge_ids = getGraph().getBackwardEdgeIDsOf(current_node);

            if constexpr(UseStallOnDemand) {
                if(shouldStall(cost_to_current, edge_ids)) {
                    continue;
                }
            }

            backward_settled_.emplace_back(current_node);

            for(const auto id : edge_ids) {
                const auto edge = getGraph().getBackwardEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getWeight();
                const auto new_dist = cost + cost_to_current;

                if(new_dist < backward_distances_[neig.get()]) {
                    heap.emplace(neig, new_dist);
                    backward_distances_[neig.get()] = new_dist;
                    backward_touched_.emplace_back(neig);
                    backward_best_ingoing_[neig.get()] = id;
                }
            }
        }

        std::sort(std::begin(backward_settled_),
                  std::end(backward_settled_));
    }

    [[nodiscard]] constexpr auto shouldStall(common::Weight cost_to_current,
                                             const std::span<const common::EdgeID>& edge_ids) const noexcept
        -> bool
    {
        for(const auto id : edge_ids) {
            const auto& edge = getGraph().getBackwardEdge(id);
            const auto neig = edge->getTrg();
            const auto cost = edge->getWeight();
            const auto current_dist_to_neig = backward_distances_[neig.get()];

            if(current_dist_to_neig == common::INFINITY_WEIGHT) {
                continue;
            }

            if(current_dist_to_neig + cost < cost_to_current) {
                return true;
            }
        }

        return false;
    }

    constexpr auto resetBackwardFor(common::NodeID node) noexcept
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

        last_source_ = node;
        backward_touched_.emplace_back(node);
        backward_distances_[node.get()] = common::Weight{0};
    }

    [[nodiscard]] constexpr auto getGraph() const noexcept
        -> decltype(auto)
    {
        return static_cast<const CRTP*>(this)->graph_;
    }

private:
    friend CRTP;
    std::vector<common::NodeID> backward_settled_;
    std::vector<common::Weight> backward_distances_;
    std::vector<common::NodeID> backward_touched_;
    std::optional<common::NodeID> last_source_;
    std::vector<common::EdgeID> backward_best_ingoing_;
    std::vector<bool> backward_already_settled_;
};

} // namespace algorithms::distoracle
