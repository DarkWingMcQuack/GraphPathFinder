#pragma once

#include <concepts/EdgeWeights.hpp>
#include <concepts/Nodes.hpp>
#include <concepts/Path.hpp>
#include <optional>

namespace concepts {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept PathOracle = requires(const O& oracle, common::NodeID src, common::NodeID trg)
{
	/**
	 * @returns an optional contaning the shortest path between src and trg, std::nullopt if no such path exists
	 */
    {oracle.pathBetween(src, trg)} noexcept -> PathOpt;
};
// clang-format on

} // namespace concepts
