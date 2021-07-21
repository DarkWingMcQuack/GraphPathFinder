#pragma once

#include <concepts>

namespace concepts {

// clang-format off
template<typename T>
concept Nodes = requires(const T& graph)
{
    typename T::NodeType; // require node type
    typename T::NodeIDType; // require node id type

    requires std::equality_comparable<typename T::NodeType>;
    requires std::totally_ordered<typename T::NodeIDType>;

    /*
	 * checks if a node with the given ID exists
	 * @return true if a node with the given ID exists, false otherwise
	 */
    {graph.nodeExists(T::NodeIDType)} noexcept -> std::same_as<bool>;

	/*
	 * returns a pointer to the node class associated with the
	 * given ID
	 * @return a non owning pointer to the node object associated with the given id,
	 * null if no such object exists
	 */
	{graph.getNode(T::NodeIDType)} noexcept -> std::same_as<const typename T::NodeType*>;

	
	/*
	 * @return the number of nodes
	 */
	{graph.numberOfNodes()} noexcept -> std::same_as<std::size_t>;
};

// clang-format on

} // namespace concepts
