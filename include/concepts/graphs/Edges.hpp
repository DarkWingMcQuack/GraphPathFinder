#pragma once

#include <span>

namespace concepts::graphs {

// clang-format off
template<typename T>
concept Edges = requires(const T& graph)
{
    typename T::EdgeType;   // require edge type
    typename T::EdgeIDType; // require edge id type

	requires std::totally_ordered<typename T::EdgeIDType>;

	/*
	 * check if an edge associated with the given edge id exists
	 * @return true if an edge associated with the given edge id exists, false otherwise
	 */
	{graph.edgeExists(T::EdgeIDType)} noexcept -> std::same_as<bool>;

	/*
	 * @return a pointer to the edge object associated with the given edge id, null if no such edge exists
	 */
	{graph.getEdge(T::EdgeIDType)} noexcept -> std::same_as<const typename T::EdgeType*>;

	
	/*
	 * @return the number of nodes
	 */
	{graph.numberOfEdges()} noexcept -> std::same_as<std::size_t>;
};

// clang-format on

} // namespace concepts::graphs
