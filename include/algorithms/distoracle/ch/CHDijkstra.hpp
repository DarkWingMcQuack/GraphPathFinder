#pragma once

#include <algorithms/distoracle/ch/CHDijkstraBackwardHelper.hpp>
#include <algorithms/distoracle/ch/CHDijkstraForwardHelper.hpp>
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

namespace algorithms::distoracle {

template<class Graph, bool UseStallOnDemand = true>
// clang-format off
requires concepts::ForwardGraph<Graph>
      && concepts::BackwardGraph<Graph>
      && concepts::ReadableNodeLevels<Graph>
      && concepts::HasEdges<Graph>
      && concepts::HasBackwardEdges<Graph>
      && concepts::HasNodes<Graph>
      && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class CHDijkstra : public CHDijkstraForwardHelper<
                       CHDijkstra<Graph, UseStallOnDemand>,
                       UseStallOnDemand>,
                   public CHDijkstraBackwardHelper<
                       CHDijkstra<Graph, UseStallOnDemand>,
                       UseStallOnDemand>
{
    using ThisType = CHDijkstra<Graph, UseStallOnDemand>;
    using ForwardHelper = CHDijkstraForwardHelper<ThisType, UseStallOnDemand>;
    using BackwardHelper = CHDijkstraBackwardHelper<ThisType, UseStallOnDemand>;

    friend ForwardHelper;
    friend BackwardHelper;

public:
    constexpr static inline bool is_threadsafe = false;


    template<bool SortGraphEdges = true>
    constexpr CHDijkstra(Graph& graph) noexcept
        : ForwardHelper(graph.numberOfNodes()),
          BackwardHelper(graph.numberOfNodes()),
          graph_(graph)
    {
        static_assert(concepts::DistanceOracle<ThisType>,
                      "CHDijkstra should fullfill the DistanceOracle concept");

        if constexpr(SortGraphEdges) {
            //sort the offeset arrays such that the ch dijkstra can be used
            graph.sortBackwardEdgeIDsAccordingTo([](const auto& g) {
                return [&](const auto lhs, const auto rhs) {
                    auto lhs_trg = g.getBackwardEdge(lhs)->getTrg();
                    auto rhs_trg = g.getBackwardEdge(rhs)->getTrg();

                    return g.getNodeLevel(lhs_trg) > g.getNodeLevel(rhs_trg);
                };
            });

            graph.sortForwardEdgeIDsAccordingTo([](const auto& g) {
                return [&](const auto lhs, const auto rhs) {
                    auto lhs_trg = g.getEdge(lhs)->getTrg();
                    auto rhs_trg = g.getEdge(rhs)->getTrg();

                    return g.getNodeLevel(lhs_trg) > g.getNodeLevel(rhs_trg);
                };
            });
        }
    }

    constexpr CHDijkstra(CHDijkstra&&) noexcept = default;
    constexpr CHDijkstra(const CHDijkstra&) noexcept = delete;

    constexpr auto operator=(CHDijkstra&&) noexcept
        -> CHDijkstra& = default;

    constexpr auto operator=(const CHDijkstra&) noexcept
        -> CHDijkstra& = delete;

    [[nodiscard]] constexpr auto distanceBetween(common::NodeID source, common::NodeID target) noexcept
        -> common::Weight
    {
        this->fillForwardInfo(source);
        this->fillBackwardInfo(target);

        const auto top_node_opt = findShortestPathCommonNode();


        if(!top_node_opt) {
            return common::INFINITY_WEIGHT;
        }



        const auto top_node = top_node_opt.value();

        return this->forward_distances_[top_node.get()]
            + this->backward_distances_[top_node.get()];
    }


private:
    [[nodiscard]] constexpr auto findShortestPathCommonNode() const noexcept
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

private:
    const Graph& graph_;
};

} // namespace algorithms::distoracle
