#pragma once

#include <concepts/graphs/Edges.hpp>
#include <concepts>
#include <optional>

namespace concepts::graphs {


// clang-format off
template<typename T>
concept Addable = requires(T lhs, T rhs)
{
    {lhs + rhs} -> std::convertible_to<T>;
};

template<typename T>
concept ReadableEdgeWeights = requires(const T& graph)
{
    requires Edges<T>;

	typename T::WeightType;

	// edge weights should be addable and totally ordered
	requires std::totally_ordered<typename T::WeightType>;
	requires Addable<typename T::WeightType>;

	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists it returns std::nullopt
	 */
	{graph.getEdgeWeight(T::EdgeIDType)} noexcept -> std::same_as<std::optional<typename T::WeightType>>;

	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists the returned value is unspecified and could be anything
	 */
	{graph.getEdgeWeightUnsafe(T::EdgeIDType)} noexcept -> std::same_as<typename T::WeightType>;

};

  
template<typename T>
concept WriteableEdgeWeights = requires(T& graph)
{
    requires ReadableEdgeWeights<T>;

	/**
	 * given an edge id, this sets the weight of the edge which is associated by the edge id
	 * if no such edge exists nothing will happen at all
	 */
	{graph.setEdgeWeight(T::EdgeIDType, T::WeightType)} noexcept -> std::same_as<void>;
};
// clang-format on

} // namespace concepts::graphs
