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
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class CRTP, bool UseStallOnDemand>
class DistanceOracleCHDijkstraForwardHelper
{
    [[nodiscard]] constexpr auto getGraph() const noexcept
        -> decltype(auto)
    {
        return static_cast<const CRTP*>(this)->getGraph();
    }

public:
    DistanceOracleCHDijkstraForwardHelper(std::size_t number_of_nodes)
        : forward_distances_(number_of_nodes, common::INFINITY_WEIGHT),
          forward_best_ingoing_(number_of_nodes, common::UNKNOWN_EDGE_ID),
          forward_already_settled_(number_of_nodes, false) {}

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

            const auto edge_ids = getGraph().getForwardEdgeIDsOf(current_node);
            const auto current_level = getGraph().getNodeLevel(current_node);

            if constexpr(UseStallOnDemand) {
                if(shouldStall(current_level, cost_to_current, edge_ids)) {
                    continue;
                }
            }

            forward_settled_.emplace_back(current_node);

            for(const auto id : edge_ids) {
                const auto edge = getGraph().getEdge(id);
                const auto neig = edge->getTrg();
                const auto cost = edge->getWeight();
                const auto neig_level = getGraph().getNodeLevel(neig);

                if(current_level >= neig_level) {
                    break;
                }

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

private:
    auto shouldStall(common::NodeLevel current_level,
                     common::Weight cost_to_current,
                     const std::span<common::EdgeID> edge_ids) const noexcept
        -> bool
    {
        auto stall_on_demand_valid = false;
        for(const auto id : edge_ids) {
            const auto& edge = getGraph().getEdge(id);
            const auto neig = edge->getTrg();
            const auto cost = edge->getWeight();

            if(current_level >= getGraph().getNodeLevel(neig)) {
                return stall_on_demand_valid;
            }

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



protected:
    std::vector<common::Weight> forward_distances_;
    std::vector<common::NodeID> forward_touched_;
    std::optional<common::NodeID> last_source_;
    std::vector<common::EdgeID> forward_best_ingoing_;
    std::vector<bool> forward_already_settled_;
    std::vector<common::NodeID> forward_settled_;
};

} // namespace algorithms::pathfinding
