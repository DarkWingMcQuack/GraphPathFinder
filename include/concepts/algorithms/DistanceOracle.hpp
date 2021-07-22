#pragma once

#include <concepts/graphs/EdgeWeights.hpp>
#include <concepts/graphs/Nodes.hpp>
#include <optional>

namespace concepts::oracles {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept DistanceOracle = requires(const O& oracle, structures::NodeID src, structures::NodeID trg)
{
	/**
	 * @returns the shortest path distance between the nodes represented by the node ids given
	 * the shortest path distance musst be calculated by cost of the path starting with the source node
	 * and ending at the target node.
	 * if no path between src and trg exists std::nullopt musst be returned
	 */
    {oracle.distanceBetween(src, trg)} noexcept -> structures::EdgeWeightOpt;

    {oracle.distanceBetweenUnsafe(src, trg)} noexcept -> structures::EdgeWeight;
};
// clang-format on

} // namespace concepts::oracles
