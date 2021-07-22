#pragma once

#include <NamedType/named_type.hpp>
#include <span>

namespace concepts::structures {


using EdgeID = fluent::NamedType<std::size_t, struct EdgeIDTag>;

// clang-format off
template<typename T>
concept Edges = requires(const T& graph, EdgeID id)
{
    typename T::EdgeType;   // require edge type

	/*
	 * check if an edge associated with the given edge id exists
	 * @return true if an edge associated with the given edge id exists, false otherwise
	 */
	{graph.edgeExists(id)} noexcept -> std::same_as<bool>;

	/*
	 * @return a pointer to the edge object associated with the given edge id, null if no such edge exists
	 */
	{graph.getEdge(id)} noexcept -> std::same_as<const typename T::EdgeType*>;

	
	/*
	 * @return the number of nodes
	 */
	{graph.numberOfEdges()} noexcept -> std::same_as<std::size_t>;
};

// clang-format on

} // namespace concepts::structures
