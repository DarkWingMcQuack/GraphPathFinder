#pragma once

#include <common/BasicGraphTypes.hpp>
#include <span>

namespace concepts::structures {

// clang-format off

template<typename Edge>
concept HasSource = requires(const Edge& edge, common::NodeID src)
{
    {edge.getSrc()} -> std::same_as<common::NodeID>;
};

template<typename Edge>
concept HasTarget = requires(const Edge& edge, common::NodeID trg)
{
    {edge.getTrg()} -> std::same_as<common::NodeID>;
};

template<typename Edge>
concept HasWeight = requires(const Edge& e, Edge& mut_edge, common::Weight weight){
    {e.getWeight()} -> std::convertible_to<common::Weight>;
    {mut_edge.setWeight(weight)} -> std::same_as<void>;
};

template<typename T>
concept HasEdges = requires(const T& graph, common::EdgeID id)
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
