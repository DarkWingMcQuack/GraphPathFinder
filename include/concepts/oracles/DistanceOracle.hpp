#pragma once

#include <concepts/graphs/EdgeWeights.hpp>
#include <concepts/graphs/Nodes.hpp>
#include <optional>

namespace concepts::oracles {

// clang-format off

//O is a distance oracle for G
template<typename O, typename G>
concept DistanceOracle = requires(const O& oracle)
{
    requires graphs::Nodes<G>;
    requires graphs::ReadableEdgeWeights<G>;
	/**
	 * checks if an edge between to nodes identified by the two given ids exists
	 * @return true if an edge between the node associated by the first argument 
	 * and the node associated by the second arument exists, false otherwise or if one of the
	 * nodes does not exists
	 */
	{oracle.distanceBetween(G::NodeIDType, G::NodeIDType)} noexcept -> std::same_as<std::optional<typename G::WeightType>>;
};
// clang-format on

} // namespace concepts::oracles
