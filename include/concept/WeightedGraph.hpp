#pragma once

#include <concept/Graph.hpp>
#include <concepts>
#include <optional>
#include <span>

namespace concepts {

// clang-format off
template<typename T>
concept WeightedGraph = requires(const T& graph)
{
    requires Graph<T>;

	typename T::WeightType;
	requires std::totally_ordered<typename T::WeightType>;

	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists it returns std::nullopt
	 */
	{graph.getEdgeWeight(T::EdgeIDType)} -> std::same_as<std::optional<typename T::WeightType>>;

	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists the returned value is unspecified and could be anything
	 */
	{graph.getEdgeWeightUnsafe(T::EdgeIDType)} -> std::same_as<typename T::WeightType>;
};
// clang-format on

} // namespace concepts
