#pragma once

#include <common/BasicGraphTypes.hpp>
#include <concepts/Edges.hpp>
#include <concepts/Utils.hpp>
#include <concepts>
#include <optional>

namespace concepts {

// clang-format off
template<typename T>
concept ReadableEdgeWeights = requires(const T& graph, common::EdgeID id)
{
    requires HasEdges<T>;

	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists it returns std::nullopt
	 */
	{graph.getEdgeWeight(id)} noexcept -> std::convertible_to<std::optional<common::Weight>>;

	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists the returned value is unspecified and could be anything
	 */
	{graph.getEdgeWeightUnsafe(id)} noexcept -> std::convertible_to<common::Weight>;

};

template<typename T>
concept WriteableEdgeWeights = requires(T& graph, common::EdgeID id, common::Weight weight)
{
    requires ReadableEdgeWeights<T>;

	/**
	 * given an edge id, this sets the weight of the edge which is associated by the edge id
	 * if no such edge exists nothing will happen at all
	 */
	{graph.setEdgeWeight(id, weight)} noexcept -> std::same_as<void>;
};
// clang-format on

} // namespace concepts
