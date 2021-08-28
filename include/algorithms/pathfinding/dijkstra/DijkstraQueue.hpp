#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

struct DijkstraQueueComparer
{
    auto operator()(const std::pair<common::NodeID, common::Weight>& lhs,
                    const std::pair<common::NodeID, common::Weight>& rhs) const noexcept
        -> bool
    {
        return lhs.second > rhs.second;
    }
};

using DijkstraQueue = std::priority_queue<std::pair<common::NodeID, common::Weight>,
                                          std::vector<std::pair<common::NodeID, common::Weight>>,
                                          DijkstraQueueComparer>;



} // namespace algorithms::pathfinding
