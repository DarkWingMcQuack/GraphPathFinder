#pragma once

#include <algorithms/distoracle/patches/Patch.hpp>
#include <algorithms/distoracle/patches/PatchLookup.hpp>
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
#include <set>
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
                const OneToOneDistanceOracle& oracle,
                const PatchLookup& lookup) noexcept
        : graph_(graph),
          oracle_(oracle),
          lookup_(lookup),
          as_source_tested_(graph_.numberOfNodes(), false),
          as_target_tested_(graph_.numberOfNodes(), false),
          is_source_(graph_.numberOfNodes(), false),
          is_target_(graph_.numberOfNodes(), false),
          barrier_(common::UNKNOWN_NODE_ID),
          all_to_barrier_(graph_.numberOfNodes(), common::INFINITY_WEIGHT),
          barrier_to_all_(graph_.numberOfNodes(), common::INFINITY_WEIGHT),
          visited_(graph_.numberOfNodes(), false)
    {}

    [[nodiscard]] auto grow(std::vector<common::NodeID> sources,
                            common::NodeID barrier,
                            std::vector<common::NodeID> targets) noexcept
        -> Patch
    {
        std::optional src_opt = sources[0];
        std::optional trg_opt = targets[0];

        setupFor(std::move(sources), barrier, std::move(targets));

        while(src_opt or trg_opt) {
            if(src_opt) {
                const auto src = src_opt.value();
                growSource(src, targets_patch_.size());
            }

            trg_opt = getNextTargetUntestedNode();
            if(trg_opt) {
                const auto trg = trg_opt.value();
                growTarget(trg, sources_patch_.size());
            }
            src_opt = getNextSourceUntestedNode();
        }

        return createPatch();
    }


private:
    auto growSource(common::NodeID node, std::size_t max_size) noexcept
        -> void
    {
        const auto candidates = srcCandidates(node, max_size);

        // iterate backwards over the explored nodes
        for(unsigned i = candidates.size(); i-- > 0;) {
            const auto current = candidates[i];
            const auto idx = current.get();

            if(as_source_tested_[idx] or is_target_[idx]) {
                continue;
            }

            // mark current as touched and as tested
            as_source_tested_[idx] = true;
            touched_.emplace_back(current);

            if(quickCheckTrg(current)) {
                continue;
            }

            // if it is easy addable or the heavy calculation is successfull
            // then current is a target
            if(couldAddSrc(current) and shouldAddSrc(current)) {
                is_source_[idx] = true;
                sources_patch_.emplace_back(current);
                maybeAddToSrcFringe(current);
            }
        }
    }

    auto growTarget(common::NodeID node, std::size_t max_size) noexcept
        -> void
    {
        const auto candidates = trgCandidates(node, max_size);

        // iterate backwards over the explored nodes
        for(unsigned i = candidates.size(); i-- > 0;) {
            const auto current = candidates[i];
            const auto idx = current.get();

            if(as_target_tested_[idx] or is_source_[idx]) {
                continue;
            }

            // mark current as touched and as tested
            as_target_tested_[idx] = true;
            touched_.emplace_back(current);

            if(quickCheckSrc(current)) {
                continue;
            }

            // if it is easy addable or the heavy calculation is successfull
            // then current is a target
            if(couldAddTrg(current) and shouldAddTrg(current)) {
                is_target_[idx] = true;
                targets_patch_.emplace_back(current);
                maybeAddToTrgFringe(current);
            }
        }
    }

    [[nodiscard]] auto srcCandidates(common::NodeID node, std::size_t max_size) noexcept
        -> std::vector<common::NodeID>
    {
        std::vector stack{node};
        pathfinding::DijkstraQueue queue;
        queue.emplace(node, common::Weight{0});

        // fill the stack with at most max_size nodes which should be considered for adding
        while(stack.size() < max_size and !queue.empty()) {
            const auto [current_node, dist] = queue.top();
            queue.pop();

            const auto incomming = graph_.getBackwardEdgeIDsOf(current_node);
            for(const auto& id : incomming) {
                const auto edge = graph_.getBackwardEdge(id);
                const auto trg = edge->getTrg();
                const auto trg_idx = trg.get();

                if(visited_[trg_idx]) {
                    continue;
                }

                visited_[trg_idx] = true;
                touched2_.emplace_back(trg_idx);
                queue.emplace(trg, dist + common::Weight{1});

                if(as_source_tested_[trg_idx]) {
                    continue;
                }

                if(is_target_[trg_idx]) {
                    continue;
                }

                stack.emplace_back(trg);
            }
        }

        for(const auto& n : touched2_) {
            visited_[n] = false;
        }
        touched2_.clear();

        return stack;
    }

    [[nodiscard]] auto trgCandidates(common::NodeID node, std::size_t max_size) noexcept
        -> std::vector<common::NodeID>
    {
        std::vector stack{node};
        pathfinding::DijkstraQueue queue;
        queue.emplace(node, common::Weight{0});

        // fill the stack with at most max_size nodes which should be considered for adding
        while(stack.size() < max_size and !queue.empty()) {
            const auto [current_node, dist] = queue.top();
            queue.pop();

            const auto outgoing = graph_.getForwardEdgeIDsOf(current_node);
            for(const auto& id : outgoing) {
                const auto* edge = graph_.getEdge(id);
                const auto trg = edge->getTrg();
                const auto trg_idx = trg.get();

                if(visited_[trg_idx]) {
                    continue;
                }

                visited_[trg_idx] = true;
                touched2_.emplace_back(trg_idx);
                queue.emplace(trg, dist + common::Weight{1});

                if(as_target_tested_[trg_idx]) {
                    continue;
                }

                if(is_source_[trg_idx]) {
                    continue;
                }

                stack.emplace_back(trg);
            }
        }

        for(const auto& n : touched2_) {
            visited_[n] = false;
        }
        touched2_.clear();

        return stack;
    }

    [[nodiscard]] auto couldAddTrg(common::NodeID node) noexcept
        -> bool
    {
        return quickCheckTrg(node) or calculateTargetAddability(node);
    }

    [[nodiscard]] auto couldAddSrc(common::NodeID node) noexcept
        -> bool
    {
        return quickCheckSrc(node) or calculateSourceAddability(node);
    }

    [[nodiscard]] auto shouldAddTrg(common::NodeID node) noexcept
        -> bool
    {
        const auto idx = node.get();
        if(barrier_to_all_[idx] == common::INFINITY_WEIGHT) {
            return false;
        }

        return std::any_of(std::begin(sources_patch_),
                           std::end(sources_patch_),
                           [&](const auto src) {
                               return lookup_.distanceBetween(src, node) == common::INFINITY_WEIGHT;
                           });
    }

    [[nodiscard]] auto shouldAddSrc(common::NodeID node) noexcept
        -> bool
    {
        const auto idx = node.get();
        if(all_to_barrier_[idx] == common::INFINITY_WEIGHT) {
            return false;
        }

        return std::any_of(std::begin(targets_patch_),
                           std::end(targets_patch_),
                           [&](const auto trg) {
                               return lookup_.distanceBetween(node, trg) == common::INFINITY_WEIGHT;
                           });
    }

    [[nodiscard]] auto quickCheckSrc(common::NodeID node) const noexcept
        -> bool
    {
        const auto incomming = graph_.getBackwardEdgeIDsOf(node);
        const auto node_to_b = all_to_barrier_[node.get()];
        return std::any_of(std::begin(incomming),
                           std::end(incomming),
                           [&](const auto& id) {
                               const auto edge = graph_.getBackwardEdge(id);
                               const auto trg = edge->getTrg();
                               const auto trg_idx = trg.get();
                               const auto to_barrier_over_node =
                                   all_to_barrier_[trg_idx] == edge->getWeight() + node_to_b;
                               return to_barrier_over_node and is_source_[trg_idx];
                           });
    }

    [[nodiscard]] auto quickCheckTrg(common::NodeID node) const noexcept
        -> bool
    {
        const auto outgoing = graph_.getForwardEdgeIDsOf(node);
        const auto b_to_node = barrier_to_all_[node.get()];
        return std::any_of(std::begin(outgoing),
                           std::end(outgoing),
                           [&](const auto& id) {
                               const auto* edge = graph_.getEdge(id);
                               const auto trg = edge->getTrg();
                               const auto trg_idx = trg.get();
                               const auto from_barrier_over_node =
                                   barrier_to_all_[trg_idx] == edge->getWeight() + b_to_node;

                               return from_barrier_over_node and is_target_[trg_idx];
                           });
    }

    [[nodiscard]] auto calculateSourceAddability(common::NodeID node) const noexcept
        -> bool
    {
        const auto to_barrier = all_to_barrier_[node.get()];

        if(to_barrier == common::INFINITY_WEIGHT) {
            return false;
        }

        return std::all_of(
            std::begin(targets_fringe_),
            std::end(targets_fringe_),
            [&](const auto trg) {
                const auto real_dist = oracle_.distanceBetween(node, trg);
                const auto over_barrier_dist = to_barrier + barrier_to_all_[trg.get()];
                return real_dist == over_barrier_dist;
            });
    }

    [[nodiscard]] auto calculateTargetAddability(common::NodeID node) const noexcept
        -> bool
    {
        const auto from_barrier = barrier_to_all_[node.get()];

        if(from_barrier == common::INFINITY_WEIGHT) {
            return false;
        }

        return std::all_of(
            std::begin(sources_fringe_),
            std::end(sources_fringe_),
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
            if(!as_source_tested_[i] and !is_target_[i]) {
                return common::NodeID{i};
            }
        }

        return std::nullopt;
    }

    [[nodiscard]] auto getNextTargetUntestedNode() const noexcept
        -> std::optional<common::NodeID>
    {
        for(std::size_t i = 0; i < as_target_tested_.size(); i++) {
            if(!as_target_tested_[i] and !is_source_[i]) {
                return common::NodeID{i};
            }
        }

        return std::nullopt;
    }

    auto maybeAddToSrcFringe(common::NodeID node) noexcept
        -> void
    {
        sources_fringe_.emplace_back(node);

        std::erase_if(sources_fringe_,
                      [&](const auto n) {
                          const auto idx = n.get();
                          const auto ids = graph_.getBackwardEdgeIDsOf(n);

                          return std::any_of(
                              std::begin(ids),
                              std::end(ids),
                              [&](const auto id) {
                                  const auto edge = graph_.getBackwardEdge(id);
                                  const auto src = edge->getTrg().get();
                                  const auto cost = edge->getWeight();

                                  if(!is_source_[src]) {
                                      return false;
                                  }

                                  return all_to_barrier_[src] == all_to_barrier_[idx] + cost;
                              });
                      });
    }

    auto maybeAddToTrgFringe(common::NodeID node) noexcept
        -> void
    {
        targets_fringe_.emplace_back(node);

        std::erase_if(targets_fringe_,
                      [&](const auto n) {
                          const auto idx = n.get();
                          const auto ids = graph_.getForwardEdgeIDsOf(n);

                          return std::any_of(
                              std::begin(ids),
                              std::end(ids),
                              [&](const auto id) {
                                  const auto* edge = graph_.getEdge(id);
                                  const auto trg = edge->getTrg().get();
                                  const auto cost = edge->getWeight();

                                  if(!is_target_[trg]) {
                                      return false;
                                  }

                                  return barrier_to_all_[trg] == barrier_to_all_[idx] + cost;
                              });
                      });
    }

    auto setupFor(std::vector<common::NodeID>&& sources,
                  common::NodeID barrier,
                  std::vector<common::NodeID>&& targets) noexcept
        -> void
    {
        sources_patch_ = std::move(sources);
        targets_patch_ = std::move(targets);
        sources_fringe_.clear();
        targets_fringe_.clear();
        barrier_ = barrier;

        for(const auto& node : touched_) {
            const auto idx = node.get();
            as_source_tested_[idx] = false;
            as_target_tested_[idx] = false;
            is_source_[idx] = false;
            is_target_[idx] = false;
        }
        touched_.clear();

        for(const auto& node : sources_patch_) {
            const auto idx = node.get();
            as_source_tested_[idx] = true;
            is_source_[idx] = true;
            maybeAddToSrcFringe(node);
            touched_.emplace_back(node);
        }

        for(const auto& node : targets_patch_) {
            const auto idx = node.get();
            as_target_tested_[idx] = true;
            is_target_[idx] = true;
            maybeAddToTrgFringe(node);
            touched_.emplace_back(node);
        }

        for(std::size_t i = 0; i < graph_.numberOfNodes(); i++) {
            const auto node = common::NodeID{i};
            const auto to_b = oracle_.distanceBetween(node, barrier_);
            const auto from_b = oracle_.distanceBetween(barrier_, node);
            all_to_barrier_[i] = to_b;
            barrier_to_all_[i] = from_b;
        }
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
    const PatchLookup& lookup_;

    std::vector<bool> as_source_tested_;
    std::vector<bool> as_target_tested_;

    std::vector<bool> is_source_;
    std::vector<bool> is_target_;

    std::vector<common::NodeID> sources_patch_;
    common::NodeID barrier_;
    std::vector<common::NodeID> targets_patch_;

    std::vector<common::NodeID> sources_fringe_;
    std::vector<common::NodeID> targets_fringe_;

    std::vector<common::Weight> all_to_barrier_;
    std::vector<common::Weight> barrier_to_all_;

    std::vector<common::NodeID> touched_;
    std::vector<std::size_t> touched2_;
    std::vector<bool> visited_;
};

} // namespace algorithms::distoracle
