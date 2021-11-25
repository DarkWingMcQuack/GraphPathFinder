#pragma once

#include <algorithms/pathfinding/dijkstra/DijkstraQueue.hpp>
#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardConnections.hpp>
#include <fmt/core.h>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::distoracle {

class Patch
{
public:
    Patch(std::vector<std::pair<common::NodeID, common::Weight>> sources,
          common::NodeID barrier,
          std::vector<std::pair<common::NodeID, common::Weight>> targets) noexcept
        : sources_(std::move(sources)),
          barrier_(barrier),
          targets_(std::move(targets)) {}

    [[nodiscard]] auto weight() const noexcept
        -> std::size_t
    {
        return sources_.size() * targets_.size();
    }

    [[nodiscard]] auto numberOfSources() const noexcept
        -> std::size_t
    {
        return sources_.size();
    }

    [[nodiscard]] auto numberOfTargets() const noexcept
        -> std::size_t
    {
        return targets_.size();
    }

    template<class Oracle>
    requires concepts::DistanceOracle<Oracle>
    [[nodiscard]] auto santityCheck(const Oracle& oracle) const noexcept
        -> bool
    {
        for(const auto& [src, to_b] : sources_) {
            for(const auto& [trg, from_b] : targets_) {
                const auto real = oracle.distanceBetween(src, trg);
                const auto added = to_b + from_b;

                const auto over_b = oracle.distanceBetween(src, barrier_) + oracle.distanceBetween(barrier_, trg);

                if(over_b != added) {
                    fmt::print("{} to {}: overb: {}, added: {}\n", src.get(), trg.get(), over_b.get(), added.get());
                    return false;
                }

                if(real != added) {
                    fmt::print("{} to {}: real: {}, added: {}\n", src.get(), trg.get(), real.get(), added.get());
                    fmt::print("{} + {} = {}\n", to_b.get(), from_b.get(), added.get());
                    return false;
                }
            }
        }

        return true;
    }

private:
    friend class PatchLookup;
    std::vector<std::pair<common::NodeID, common::Weight>> sources_;
    common::NodeID barrier_;
    std::vector<std::pair<common::NodeID, common::Weight>> targets_;
};
} // namespace algorithms::distoracle
