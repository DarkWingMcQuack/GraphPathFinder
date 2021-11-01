#pragma once

#include <algorithms/pathfinding/ch/CHDijkstraBackwardHelper.hpp>
#include <algorithms/pathfinding/ch/CHDijkstraForwardHelper.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/BackwardConnections.hpp>
#include <concepts/BackwardEdges.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <concepts/NodeLevels.hpp>
#include <concepts/PathOracle.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

template<class Graph, bool UseStallOnDemand = true>
// clang-format off
  requires concepts::ForwardConnections<Graph>
  && concepts::BackwardConnections<Graph>
  && concepts::ReadableNodeLevels<Graph>
  && concepts::HasEdges<Graph>
  && concepts::HasBackwardEdges<Graph>
  && concepts::HasNodes<Graph>
  && concepts::HasTarget<typename Graph::EdgeType>
  && concepts::CanUnwrapShortcuts<Graph>
// clang-format on
class CHDijkstra : public CHDijkstraForwardHelper<
                       CHDijkstra<Graph, UseStallOnDemand>,
                       UseStallOnDemand>,
                   public CHDijkstraBackwardHelper<
                       CHDijkstra<Graph, UseStallOnDemand>,
                       UseStallOnDemand>
{
    using ForwardHelper = CHDijkstraForwardHelper<CHDijkstra, UseStallOnDemand>;
    using BackwardHelper = CHDijkstraBackwardHelper<CHDijkstra, UseStallOnDemand>;
    friend ForwardHelper;
    friend BackwardHelper;

public:
    constexpr static inline bool is_threadsafe = false;


    template<bool SortGraphEdges = true>
    constexpr CHDijkstra(const Graph& graph) noexcept
        : ForwardHelper(graph.numberOfNodes()),
          BackwardHelper(graph.numberOfNodes()),
          graph_(graph)
    {
        static_assert(concepts::DistanceOracle<CHDijkstra>,
                      "CHDijkstra should fullfill the DistanceOracle concept");

        static_assert(concepts::PathOracle<CHDijkstra>,
                      "Dijkstra should fullfill the PathOracle concept");
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

    auto pathBetween(common::NodeID source, common::NodeID target) noexcept
        -> std::optional<graphs::Path>
    {
        this->fillForwardInfo(source);
        this->fillBackwardInfo(target);

        const auto top_node_opt = findShortestPathCommonNode();

        if(!top_node_opt) {
            return std::nullopt;
        }

        const auto top_node = top_node_opt.value();
        const auto edge_path = calculateEdgePath(top_node, source, target);
        return graph_.buildPathFromEdges(edge_path);
    }


private:
    [[nodiscard]] auto calculateEdgePath(common::NodeID top_node,
                                         common::NodeID src,
                                         common::NodeID trg) const noexcept
        -> std::vector<common::EdgeID>
    {
        std::vector<common::EdgeID> edge_path;

        common::NodeID current = top_node;
        while(current != src) {
            const auto curr_best_ingoing = this->forward_best_ingoing_[current.get()];
            edge_path.emplace_back(curr_best_ingoing);
            current = graph_.getEdge(curr_best_ingoing)->getSrc();
        }

        std::reverse(std::begin(edge_path), std::end(edge_path));

        current = top_node;
        while(current != trg) {
            const auto curr_best_ingoing = this->backward_best_ingoing_[current.get()];
            edge_path.emplace_back(curr_best_ingoing);
            current = graph_.getBackwardEdge(curr_best_ingoing)->getSrc();
        }

        return edge_path;
    }

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

        if(best_dist == common::INFINITY_WEIGHT) {
            return std::nullopt;
        }

        return best_node;
    }

private:
    const Graph& graph_;
};



// clang-format off
template<class Graph>
requires concepts::DeletableForwardConnections<Graph> &&
         concepts::DeletableBackwardConnections<Graph>
// clang-format on
[[nodiscard]] constexpr inline auto prepareGraphForCHDijkstra(Graph&& g) noexcept
    -> Graph
{
    // the graph should only return valid edges for the forward and backward search
    g.deleteForwardEdgesIDsIf([](const auto& graph) {
        return [&](const auto id) {
            const auto* edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });

    g.deleteBackwardEdgesIDsIf([](const auto& graph) {
        return [&](const auto id) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });

    return g;
}

} // namespace algorithms::pathfinding
