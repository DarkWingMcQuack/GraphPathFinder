#pragma once

#include <common/BasicGraphTypes.hpp>
#include <concepts>

namespace concepts::structures {

// clang-format off
template<typename Node>
concept HasLevel = requires(const Node& node, Node& mut_node, common::NodeLevel level)
{
    {node.getLvl()} -> std::convertible_to<common::NodeLevel>;
    {mut_node.setLvl(level)} -> std::same_as<void>;
};

template<typename T>
concept HasNodes = requires(const T& graph, common::NodeID id)
{
    typename T::NodeType; // require node type

    requires std::equality_comparable<typename T::NodeType>;

    /*
	 * checks if a node with the given ID exists
	 * @return true if a node with the given ID exists, false otherwise
	 */
    {graph.nodeExists(id)} noexcept -> std::same_as<bool>;

	/*
	 * returns a pointer to the node class associated with the
	 * given ID
	 * @return a non owning pointer to the node object associated with the given id,
	 * null if no such object exists
	 */
	{graph.getNode(id)} noexcept -> std::same_as<const typename T::NodeType*>;

	
	/*
	 * @return the number of nodes
	 */
	{graph.numberOfNodes()} noexcept -> std::same_as<std::size_t>;
};

// clang-format on

} // namespace concepts::structures