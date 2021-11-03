#pragma once

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
#include <concepts/Permutable.hpp>
#include <execution>
#include <fmt/core.h>
#include <numeric>
#include <queue>
#include <type_traits>
#include <utility>
#include <utils/Permutation.hpp>

namespace algorithms::distoracle {

class HubLabelLookup
{
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
    friend class HubLabelCalculator;

    using HubType = std::pair<common::NodeID, common::Weight>;

    // private ctor because a HubLabelLookup should only be consructed via a HubLabelCalculator
    HubLabelLookup(std::vector<std::vector<HubType>> in_labels,
                   std::vector<std::vector<HubType>> out_labels) noexcept
        : in_labels_(std::move(in_labels)),
          out_labels_(std::move(out_labels))
    {
        static_assert(concepts::DistanceOracle<HubLabelLookup>,
                      "HubLabelLookup should fullfill the DistanceOracle concept");

        static_assert(concepts::NodesPermutable<HubLabelLookup>,
                      "HubLabelLookup should be able to permutate nodes");
    }


    [[nodiscard]] static auto distanceOracle(const std::vector<HubType>& out_l,
                                             const std::vector<HubType>& in_l) noexcept
        -> common::Weight
    {
        const auto max_s_size = out_l.size();
        const auto max_t_size = in_l.size();

        auto best_node = common::UNKNOWN_NODE_ID;
        auto best_dist = common::INFINITY_WEIGHT;

        auto s_idx = 0ul;
        auto t_idx = 0ul;

        while(s_idx < max_s_size and t_idx < max_t_size) {
            const auto src_hub = out_l[s_idx];
            const auto trg_hub = in_l[t_idx];

            if(src_hub.first == trg_hub.first) {
                const auto tmp_dist = src_hub.second + trg_hub.second;
                if(tmp_dist < best_dist) {
                    best_dist = tmp_dist;
                    best_node = src_hub.first;
                }
                s_idx++;
                t_idx++;
            } else if(src_hub.first < trg_hub.first) {
                s_idx++;
            } else {
                t_idx++;
            }
        }

        return best_dist;
    }

public:
    constexpr static inline bool is_threadsafe = false;

    [[nodiscard]] auto distanceBetween(common::NodeID source, common::NodeID target) const noexcept
        -> common::Weight
    {
        const auto& in_l = in_labels_[target.get()];
        const auto& out_l = out_labels_[source.get()];

        return HubLabelLookup::distanceOracle(out_l, in_l);
    }

    [[nodiscard]] auto numberOfNodes() const noexcept
        -> std::size_t
    {
        return in_labels_.size();
    }

    auto applyNodePermutation(std::vector<std::size_t> perm,
                              const std::vector<std::size_t>& inv_perm) noexcept
        -> bool
    {
        // clang-format: off
        if(in_labels_.size() != perm.size()
           or out_labels_.size() != perm.size()
           or inv_perm.size() != perm.size()) {
            return false;
        }
        // clang-format: on

        // curried function expecting a permutation vector to produce
        // the function which updates the nodes in the in and out labels
        static const auto transform_f =
            [](const auto& inv_perm) {
                return [&](auto& l) {
                    std::transform(
                        std::execution::unseq,
                        std::begin(l),
                        std::end(l),
                        std::begin(l),
                        [&](const auto hub) {
                            const auto [node, dist] = hub;
                            const auto new_position = inv_perm[node.get()];
                            const auto new_node = common::NodeID{new_position};

                            return std::pair{new_node, dist};
                        });
                    std::sort(std::begin(l), std::end(l));
                };
            };

        std::for_each(std::execution::par,
                      std::begin(in_labels_),
                      std::end(in_labels_),
                      transform_f(inv_perm));

        std::for_each(std::execution::par,
                      std::begin(out_labels_),
                      std::end(out_labels_),
                      transform_f(inv_perm));

        in_labels_ = util::applyPermutation(std::move(in_labels_), perm);
        out_labels_ = util::applyPermutation(std::move(out_labels_), std::move(perm));

        return true;
    }

private:
    std::vector<std::vector<HubType>> in_labels_;
    std::vector<std::vector<HubType>> out_labels_;
};

} // namespace algorithms::distoracle
