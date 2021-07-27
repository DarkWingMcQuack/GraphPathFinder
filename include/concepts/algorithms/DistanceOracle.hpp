#pragma once

#include <concepts/structures/EdgeWeights.hpp>
#include <concepts/structures/Nodes.hpp>
#include <optional>

namespace concepts::oracles {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept DistanceOracle = requires(const O& oracle, common::NodeID src, common::NodeID trg)
{
	/**
	 * @returns the shortest path distance between the nodes represented by the node ids given
	 * the shortest path distance musst be calculated by cost of the path starting with the source node
	 * and ending at the target node.
	 * if no path between src and trg exists std::nullopt musst be returned
	 */
    {oracle.distanceBetween(src, trg)} noexcept -> std::same_as<std::optional<common::EdgeWeight>>;

    {oracle.distanceBetweenUnsafe(src, trg)} noexcept -> std::same_as<common::EdgeWeight>;
};
// clang-format on

} // namespace concepts::oracles
