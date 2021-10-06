#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <queue>
#include <type_traits>
#include <utility>

namespace algorithms::pathfinding {

using NodeLabel = std::pair<common::NodeID, common::Weight>;

struct DijkstraQueueComparer
{
    constexpr auto operator()(const NodeLabel& lhs,
                              const NodeLabel& rhs) const noexcept
        -> bool
    {
        return lhs.second > rhs.second;
    }
};

using DijkstraQueue = std::priority_queue<NodeLabel,
                                          std::vector<NodeLabel>,
                                          DijkstraQueueComparer>;



} // namespace algorithms::pathfinding
