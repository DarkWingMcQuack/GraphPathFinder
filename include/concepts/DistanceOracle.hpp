#pragma once

#include <concepts/Edges.hpp>
#include <concepts/Nodes.hpp>
#include <optional>

namespace concepts {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept DistanceOracle = requires(O& oracle, common::NodeID src, common::NodeID trg)
{
	/**
	 * @returns the shortest path distance between the nodes represented by the node ids given
	 * the shortest path distance musst be calculated by cost of the path starting with the source node
	 * and ending at the target node.
	 * if no path between src and trg exists INFINITY_WEIGHT musst be returned
	 */
    {oracle.distanceBetween(src, trg)} noexcept -> std::same_as<common::Weight>;

	/**
	 * @returns true if the distance oracle can be used in a multithreaded environment,
	 * false otherwise
	 */
	{O::is_threadsafe} noexcept -> std::common_with<const bool>;
};

template<typename O>
concept OneToManyDistanceOracle = requires(O& oracle, common::NodeID src)
{
	/**
	 * @returns a vector of distances to all other nodes
	 */
    {oracle.distancesFrom(src)} noexcept -> std::same_as<const std::vector<common::Weight>&>;

	/**
	 * @returns true if the distance oracle can be used in a multithreaded environment,
	 * false otherwise
	 */
	{O::is_threadsafe} noexcept -> std::common_with<const bool>;
};
// clang-format on

} // namespace concepts
