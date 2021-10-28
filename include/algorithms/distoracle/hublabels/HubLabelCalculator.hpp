#pragma once

#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/BackwardConnections.hpp>
#include <concepts/BackwardEdges.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <concepts/NodeLevels.hpp>
#include <concepts/Nodes.hpp>
#include <concepts/SortableGraph.hpp>

namespace algorithms::distoracle {

template<class Graph>
// clang-format off
  requires concepts::ForwardConnections<Graph>
  && concepts::BackwardConnections<Graph>
  && concepts::ReadableNodeLevels<Graph>
  && concepts::HasEdges<Graph>
  && concepts::HasBackwardEdges<Graph>
  && concepts::HasNodes<Graph>
  && concepts::HasTarget<typename Graph::EdgeType>
// clang-format on
class HubLabelCalculator
{
public:
    constexpr HubLabelCalculator(const Graph& graph) noexcept
        : graph_(graph),
          in_labels_(graph_.numberOfNodes()),
          out_labels_(graph_.numberOfNodes()) {}

private:
    [[nodiscard]] auto constructOutLabels(common::NodeID node) const noexcept
        -> std::vector<HubLabelLookup::HubType>
    {
        std::vector<HubLabelLookup::HubType> hubs;
        hubs.emplace_back(node, common::Weight{0});
        for(const auto edge_id : graph_.getForwardEdgeIDsOf(node)) {
            const auto* edge = graph_.getEdge(edge_id);
            const auto trg = edge->getTrg();

            const auto weight = [&]() constexpr
            {
                if constexpr(concepts::HasWeight<typename Graph::EdgeType>) {
                    return edge->getWeight();
                } else {
                    return common::Weight{1};
                }
            }
            ();

            hubs.emplace_back(trg, weight);

            const auto& trg_out_hubs = out_labels_[trg.get()];
            std::transform(std::begin(trg_out_hubs),
                           std::end(trg_out_hubs),
                           std::back_inserter(hubs),
                           [&](const auto hub) {
                               const auto [hub_node, hub_dist] = hub;
                               return std::pair{hub_node, hub_dist + weight};
                           });
        }

        std::sort(std::begin(hubs),
                  std::end(hubs),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.first < rhs.first;
                  });

        return hubs;
    }

    constexpr auto pruneOutLabels(common::NodeID node, std::vector<HubLabelLookup::HubType>& hubs) const noexcept
        -> void
    {
        hubs.erase(std::unique(hubs.begin(), hubs.end()),
                   hubs.end());

        std::erase_if(hubs, [&](const auto& hub) {
            const auto trg = hub.first;
            const auto label_dist = hub.second;
            const auto real_dist = [&] {
                if(node == trg) {
                    return common::Weight{0};
                }

                const auto& in_hubs = in_labels_[trg.get()];
                return HubLabelLookup::distanceOracle(hubs, in_hubs);
            }();

            return label_dist > real_dist;
        });
    }

    [[nodiscard]] auto constructInLabels(common::NodeID node) const noexcept
        -> std::vector<HubLabelLookup::HubType>
    {
        std::vector<HubLabelLookup::HubType> hubs;
        hubs.emplace_back(node, common::Weight{0});
        for(const auto edge_id : graph_.getBackwardEdgeIDsOf(node)) {
            const auto edge = graph_.getBackwardEdge(edge_id);
            const auto trg = edge->getTrg();

            const auto weight = [&]() constexpr
            {
                if constexpr(concepts::HasWeight<typename Graph::EdgeType>) {
                    return edge->getWeight();
                } else {
                    return common::Weight{1};
                }
            }
            ();

            hubs.emplace_back(trg, weight);

            const auto& trg_in_hubs = in_labels_[trg.get()];
            std::transform(std::begin(trg_in_hubs),
                           std::end(trg_in_hubs),
                           std::back_inserter(hubs),
                           [&](const auto hub) {
                               const auto [hub_node, hub_dist] = hub;
                               return std::pair{hub_node, hub_dist + weight};
                           });
        }

        std::sort(std::begin(hubs),
                  std::end(hubs),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.first < rhs.first;
                  });

        return hubs;
    }

    constexpr auto pruneInLabels(common::NodeID node, std::vector<HubLabelLookup::HubType>& labels) const noexcept
        -> void
    {
        labels.erase(std::unique(labels.begin(), labels.end()),
                     labels.end());

        std::erase_if(labels, [&](const auto& label) {
            const auto trg = label.first;
            const auto label_dist = label.second;
            const auto real_dist = [&] {
                if(node == trg) {
                    return common::Weight{0};
                }

                const auto& out_hubs = out_labels_[trg.get()];
                return HubLabelLookup::distanceOracle(out_hubs, labels);
            }();

            return label_dist > real_dist;
        });
    }

public:
    [[nodiscard]] auto constructHubLabelLookup() noexcept
        -> HubLabelLookup
    {
        for(std::size_t i = 0; i < graph_.numberOfNodes(); i++) {
            common::NodeID current_node{i};

            auto in = constructInLabels(current_node);
            pruneInLabels(current_node, in);
            in_labels_[i] = std::move(in);

            auto out = constructOutLabels(current_node);
            pruneOutLabels(current_node, out);
            out_labels_[i] = std::move(out);
        }

        return HubLabelLookup{std::move(in_labels_),
                              std::move(out_labels_)};
    }

    [[nodiscard]] auto constructHubLabelsInParallel() noexcept
        -> HubLabelLookup
    {}

private:
    const Graph& graph_;
    std::vector<std::vector<HubLabelLookup::HubType>> in_labels_;
    std::vector<std::vector<HubLabelLookup::HubType>> out_labels_;
};


// clang-format off
template<class Graph>
requires concepts::DeletableForwardConnections<Graph> &&
concepts::DeletableBackwardConnections<Graph> &&
concepts::NodesSortable<Graph>
// clang-format on
[[nodiscard]] inline auto prepareGraphForHubLabelCalculator(Graph g) noexcept
    -> Graph
{
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

    g.sortNodesAccordingTo([](const auto& graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_lvl = graph.getNodeLevelUnsafe(lhs);
            const auto rhs_lvl = graph.getNodeLevelUnsafe(rhs);
            return lhs_lvl > rhs_lvl;
        };
    });

    return g;
}

} // namespace algorithms::distoracle
