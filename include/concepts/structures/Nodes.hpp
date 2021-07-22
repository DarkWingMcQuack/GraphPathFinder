#pragma once

#include <NamedType/named_type.hpp>
#include <concepts>

namespace concepts::structures {

using NodeID = fluent::NamedType<std::size_t, struct NodeIDTag>;

// clang-format off
template<typename T>
concept Nodes = requires(const T& graph, NodeID id)
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
