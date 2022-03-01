#pragma once

#include <algorithms/distoracle/patches/Patch.hpp>
#include <algorithms/distoracle/patches/PatchLookup.hpp>
#include <algorithms/distoracle/patches/quadtree/QuadTree.hpp>
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

namespace algorithms {

struct WellSeparatedPair
{
    WellSeparatedPair(const impl::QuadTreeNode& first,
                      const impl::QuadTreeNode& second) noexcept
        : box1_(first.getBoundingBox()),
          box2_(second.getBoundingBox()),
          elements1_(first.getElements()),
          elements2_(second.getElements()) {}

    [[nodiscard]] constexpr auto operator==(const WellSeparatedPair& rhs) const noexcept
        -> bool
    {
        return box1_ == rhs.box1_ and box2_ == rhs.box2_;
    }

    impl::BoundingBox box1_;
    impl::BoundingBox box2_;
    std::span<const common::NodeID> elements1_;
    std::span<const common::NodeID> elements2_;
};

inline auto calulateWSPDRecHelper(const impl::QuadTreeNode& first,
                                  const impl::QuadTreeNode& second,
                                  double epsilon,
                                  std::vector<WellSeparatedPair>& result) noexcept
    -> void
{
    if(first == second) {
        return;
    }

	// well separation check
    if(first.diam() <= epsilon * first.distanceTo(second)) {
        result.emplace_back(first, second);
        return;
    }

    if(first.getBoundingBox() < second.getBoundingBox()) {
        calulateWSPDRecHelper(first, *second.getTopLeftChild(), epsilon, result);
        calulateWSPDRecHelper(first, *second.getTopRightChild(), epsilon, result);
        calulateWSPDRecHelper(first, *second.getBottomLeftChild(), epsilon, result);
        calulateWSPDRecHelper(first, *second.getBottomRightChild(), epsilon, result);
    } else {
        calulateWSPDRecHelper(second, *first.getTopLeftChild(), epsilon, result);
        calulateWSPDRecHelper(second, *first.getTopRightChild(), epsilon, result);
        calulateWSPDRecHelper(second, *first.getBottomLeftChild(), epsilon, result);
        calulateWSPDRecHelper(second, *first.getBottomRightChild(), epsilon, result);
    }
}

inline auto calculateWSPD(QuadTree& tree, double epsilon) noexcept
    -> std::vector<WellSeparatedPair>
{
    std::vector<WellSeparatedPair> wspd;
    calulateWSPDRecHelper(tree.getRoot(),
                          tree.getRoot(),
                          epsilon,
                          wspd);
    return wspd;
}

} // namespace algorithms
