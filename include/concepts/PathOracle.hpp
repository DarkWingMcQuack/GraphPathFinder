#pragma once

#include <concepts/EdgeWeights.hpp>
#include <concepts/Nodes.hpp>
#include <concepts/Path.hpp>
#include <graphs/Path.hpp>
#include <optional>

namespace concepts {

// clang-format off

//O is a distance oracle for G
template<typename O>
concept PathOracle = requires(O& oracle, common::NodeID src, common::NodeID trg)
{
	/**
	 * @returns an optional contaning the shortest path between src and trg, std::nullopt if no such path exists
	 */
    {oracle.pathBetween(src, trg)} noexcept -> std::same_as<std::optional<graphs::Path>>;

	
	/**
	 * @returns true if the path oracle can be used in a multithreaded environment,
	 * false otherwise
	 */
	{O::is_threadsafe} noexcept -> std::common_with<const bool>;
};
// clang-format on

} // namespace concepts
