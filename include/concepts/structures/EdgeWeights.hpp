#pragma once

#include <concepts/Utils.hpp>
#include <concepts/structures/Edges.hpp>
#include <concepts>
#include <optional>

namespace concepts::structures {


// clang-format off
template<typename T>
concept EdgeWeight = requires(T lhs, T rhs)
{
    {lhs + rhs} -> std::convertible_to<T>;
	requires std::totally_ordered<T>;
};

template<typename T>
concept EdgeWeightOpt = requires
{
    typename T::value_type;
	requires EdgeWeight<typename T::value_type>;
	requires is_optional<T>;
};

  


template<typename T>
concept ReadableEdgeWeights = requires(const T& graph, EdgeID id)
{
    requires Edges<T>;

	//a graph T which wants to fullfill this concept needs a nested type WeightType
	//which can be used as weight type and should be addable and totally ordered
	typename T::WeightType;
	// edge weights should be addable and totally ordered
	requires EdgeWeight<typename T::WeightType>;

	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists it returns std::nullopt
	 */
	{graph.getEdgeWeight(id)} noexcept -> std::same_as<std::optional<typename T::WeightType>>;

	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists the returned value is unspecified and could be anything
	 */
	{graph.getEdgeWeightUnsafe(id)} noexcept -> std::same_as<typename T::WeightType>;

};

  
template<typename T>
concept WriteableEdgeWeights = requires(T& graph, EdgeID id, typename T::WeightType weight)
{
    requires ReadableEdgeWeights<T>;

	/**
	 * given an edge id, this sets the weight of the edge which is associated by the edge id
	 * if no such edge exists nothing will happen at all
	 */
	{graph.setEdgeWeight(id, weight)} noexcept -> std::same_as<void>;
};
// clang-format on

} // namespace concepts::structures
