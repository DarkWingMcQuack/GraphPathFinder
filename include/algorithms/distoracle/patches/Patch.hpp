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

private:
    std::vector<std::pair<common::NodeID, common::Weight>> sources_;
    common::NodeID barrier_;
    std::vector<std::pair<common::NodeID, common::Weight>> targets_;
};
} // namespace algorithms::distoracle
