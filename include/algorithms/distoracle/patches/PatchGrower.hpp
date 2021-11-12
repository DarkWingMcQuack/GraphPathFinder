#pragma once

#include <algorithms/distoracle/patches/Patch.hpp>
#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/BackwardConnections.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <concepts/Nodes.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

// clang-format off
template<class Graph, class OneToOneDistanceOracle>
requires concepts::HasNodes<Graph> &&
         concepts::HasEdges<Graph> &&
         concepts::HasSource<typename Graph::EdgeType> &&
         concepts::HasTarget<typename Graph::EdgeType> &&
         concepts::ForwardConnections<Graph> &&
         concepts::BackwardConnections<Graph> &&
         concepts::DistanceOracle<OneToOneDistanceOracle>
// clang-format on
class PatchGrower
{
public:
    PatchGrower(const Graph& graph,
                const OneToOneDistanceOracle& oracle) noexcept
        : graph_(graph),
          oracle_(oracle),
          as_source_tested_(graph_.numberOfNodes(), false),
          as_target_tested_(graph_.numberOfNodes(), false),
          is_source_(graph_.numberOfNodes(), false),
          is_target_(graph_.numberOfNodes(), false),
          barrier_(common::UNKNOWN_NODE_ID),
          all_to_barrier_(graph_.numberOfNodes(), common::INFINITY_WEIGHT),
          barrier_to_all_(graph_.numberOfNodes(), common::INFINITY_WEIGHT)
    {}

    [[nodiscard]] auto grow(std::vector<common::NodeID> sources,
                            common::NodeID barrier,
                            std::vector<common::NodeID> targets) noexcept
        -> Patch
    {
        setupFor(std::move(sources), barrier, std::move(targets));

        auto src_opt = getNextSourceUntestedNode();
        auto trg_opt = getNextTargetUntestedNode();

        while(src_opt or trg_opt) {
            if(src_opt) {
                const auto src = src_opt.value();
                growSource(src);
            }

            if(trg_opt) {
                const auto trg = trg_opt.value();
                growTarget(trg);
            }

            src_opt = getNextSourceUntestedNode();
            trg_opt = getNextTargetUntestedNode();
        }

        return createPatch();
    }


private:
    [[nodiscard]] auto growSource(common::NodeID node) noexcept
        -> bool
    {
        const auto idx = node.get();

        // if the node is already a src or was already tested
        // we do not try to add it as source
        if(is_target_[idx]) {
            return false;
        }

        if(as_source_tested_[idx]) {
            return is_source_[idx];
        }

        // check if all the incomming neigbours are also srcs if so
        // the current node is also a src
        const auto incomming = graph_.getBackwardEdgeIDsOf(node);
        const auto easy_addable = std::any_of(
            std::begin(incomming),
            std::end(incomming),
            [&](const auto id) {
                const auto edge = graph_.getBackwardEdge(id);
                const auto trg = edge->getTrg();

                return growSource(trg);
            });

        if(easy_addable) {
            fmt::print("add src without calculation...\n");
        }

        // add node if possible and only calculate the distances if it is not easy addable
        if(easy_addable or calculateSourceAddability(node)) {
            is_source_[idx] = true;
            sources_patch_.emplace_back(node);
            all_to_barrier_[idx] = oracle_.distanceBetween(node, barrier_);
        }

        // set that this src was already checked
        as_source_tested_[idx] = true;
        touched_.emplace_back(node);

        return is_source_[idx];
    }

    [[nodiscard]] auto growTarget(common::NodeID node) noexcept
        -> bool
    {
        const auto idx = node.get();

        // if the node is already a src or was already tested
        // we do not try to add it as source
        if(is_source_[idx]) {
            return false;
        }

        if(as_target_tested_[idx]) {
            return is_target_[idx];
        }

        // check if all the incomming neigbours are also srcs if so
        // the current node is also a src
        const auto incomming = graph_.getForwardEdgeIDsOf(node);
        const auto easy_addable = std::any_of(
            std::begin(incomming),
            std::end(incomming),
            [&](const auto id) {
                const auto* edge = graph_.getEdge(id);
                const auto trg = edge->getTrg();

                return growTarget(trg);
            });

        if(easy_addable) {
            fmt::print("add trg without calculation...\n");
        }

        // add node if possible and only calculate the distances if it is not easy addable
        // if the node is added then add the distance from the barrier to the distance vector
        if(easy_addable or calculateTargetAddability(node)) {
            is_target_[idx] = true;
            targets_patch_.emplace_back(node);
            barrier_to_all_[idx] = oracle_.distanceBetween(barrier_, node);
        }

        // set that this src was already checked
        as_target_tested_[idx] = true;
        touched_.emplace_back(node);

        return is_target_[idx];
    }

    [[nodiscard]] auto calculateSourceAddability(common::NodeID node) const noexcept
        -> bool
    {
        fmt::print("check src with calculation...\n");
        const auto to_barrier = all_to_barrier_[node.get()];
        return std::all_of(
            std::begin(targets_patch_),
            std::end(targets_patch_),
            [&](const auto trg) {
                const auto real_dist = oracle_.distanceBetween(node, trg);
                const auto over_barrier_dist = to_barrier + barrier_to_all_[trg.get()];
                return real_dist == over_barrier_dist;
            });
    }

    [[nodiscard]] auto calculateTargetAddability(common::NodeID node) const noexcept
        -> bool
    {
        fmt::print("check trg with calculation...\n");
        const auto from_barrier = barrier_to_all_[node.get()];
        return std::all_of(
            std::begin(sources_patch_),
            std::end(sources_patch_),
            [&](const auto src) {
                const auto real_dist = oracle_.distanceBetween(src, node);
                const auto over_barrier_dist = all_to_barrier_[src.get()] + from_barrier;
                return real_dist == over_barrier_dist;
            });
    }

    [[nodiscard]] auto getNextSourceUntestedNode() const noexcept
        -> std::optional<common::NodeID>
    {
        for(std::size_t i = 0; i < as_source_tested_.size(); i++) {
            if(!as_source_tested_[i]) {
                return common::NodeID{i};
            }
        }

        return std::nullopt;
    }

    [[nodiscard]] auto getNextTargetUntestedNode() const noexcept
        -> std::optional<common::NodeID>
    {
        for(std::size_t i = 0; i < as_source_tested_.size(); i++) {
            if(!as_target_tested_[i]) {
                return common::NodeID{i};
            }
        }

        return std::nullopt;
    }

    auto setupFor(std::vector<common::NodeID>&& sources,
                  common::NodeID barrier,
                  std::vector<common::NodeID>&& targets) noexcept
        -> void
    {
        sources_patch_ = std::move(sources);
        targets_patch_ = std::move(targets);
        barrier_ = barrier;

        for(const auto& node : touched_) {
            const auto idx = node.get();
            as_source_tested_[idx] = false;
            as_target_tested_[idx] = false;
            is_source_[idx] = false;
            is_target_[idx] = false;
            all_to_barrier_[idx] = common::INFINITY_WEIGHT;
            barrier_to_all_[idx] = common::INFINITY_WEIGHT;
        }

        touched_.clear();
    }

    [[nodiscard]] auto createPatch() const noexcept
        -> Patch
    {
        std::vector<std::pair<common::NodeID, common::Weight>> sources;
        sources.reserve(sources_patch_.size());
        std::transform(std::begin(sources_patch_),
                       std::end(sources_patch_),
                       std::back_inserter(sources),
                       [&](const auto node) {
                           const auto distance = all_to_barrier_[node.get()];
                           return std::pair{node, distance};
                       });

        std::vector<std::pair<common::NodeID, common::Weight>> targets;
        targets.reserve(targets_patch_.size());
        std::transform(std::begin(targets_patch_),
                       std::end(targets_patch_),
                       std::back_inserter(targets),
                       [&](const auto node) {
                           const auto distance = barrier_to_all_[node.get()];
                           return std::pair{node, distance};
                       });

        return Patch{std::move(sources),
                     barrier_,
                     std::move(targets)};
    }


private:
    const Graph& graph_;
    const OneToOneDistanceOracle& oracle_;

    std::vector<bool> as_source_tested_;
    std::vector<bool> as_target_tested_;

    std::vector<bool> is_source_;
    std::vector<bool> is_target_;

    std::vector<common::NodeID> sources_patch_;
    common::NodeID barrier_;
    std::vector<common::NodeID> targets_patch_;

    std::vector<common::Weight> all_to_barrier_;
    std::vector<common::Weight> barrier_to_all_;

    std::vector<common::NodeID> touched_;
};

} // namespace algorithms::distoracle
