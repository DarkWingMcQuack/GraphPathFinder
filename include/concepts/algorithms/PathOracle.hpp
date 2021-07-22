#pragma once

#include <concepts/structures/EdgeWeights.hpp>
#include <concepts/structures/Nodes.hpp>
#include <concepts/structures/Path.hpp>
#include <optional>

namespace concepts::algorithms {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept PathOracle = requires(const O& oracle, structures::NodeID src, structures::NodeID trg)
{
	/**
	 * @returns an optional contaning the shortest path between src and trg, std::nullopt if no such path exists
	 */
    {oracle.pathBetween(src, trg)} noexcept -> structures::PathOpt;
};
// clang-format on

} // namespace concepts::algorithms
