#pragma once

#include <concepts/Nodes.hpp>
#include <concepts>
#include <optional>
#include <span>

namespace concepts {


// clang-format off
template<typename T>
concept ReadableNodeLevels = requires(const T& graph, common::NodeID id)
{
    requires HasNodes<T>;

	/**
	 * given an node id, this returns the level of the node which is associated by the node id
	 * if no such node exists it returns std::nullopt
	 */
	{graph.getNodeLevel(id)} noexcept -> std::convertible_to<std::optional<common::NodeLevel>>;
	
	/**
	 * given an edge id, this returns the weight of the edge which is associated by the edge id
	 * if no such edge exists the returned value is unspecified and could be anything
	 */
	{graph.getNodeLevelUnsafe(id)} noexcept -> std::convertible_to<common::NodeLevel>;
};

  
template<typename T>
concept WriteableNodeLevels = requires(T& graph, common::NodeID id, common::NodeLevel level)
{
    requires ReadableNodeLevels<T>;

	/**
	 * given an node id, this sets the level of the node which is associated by the node id
	 * if no such node exists nothing will happen at all
	 */
	{graph.setNodeLevel(id, level)} noexcept -> std::same_as<void>;
};
// clang-format on

} // namespace concepts
