#pragma once

#include <common/BasicGraphTypes.hpp>
#include <span>

namespace concepts {

// clang-format off

template<typename Edge>
concept HasSource = requires(const Edge& edge, Edge& e,common::NodeID n)
{
    {edge.getSrc()} -> std::same_as<common::NodeID>;
    {e.setSrc(n)} -> std::same_as<void>;
};

template<typename Edge>
concept HasTarget = requires(const Edge& edge, Edge& e, common::NodeID trg)
{
    {edge.getTrg()} -> std::same_as<common::NodeID>;
    {e.setTrg(trg)} -> std::same_as<void>;
};

template<typename Edge>
concept HasWeight = requires(const Edge& e, common::Weight w){
    {e.getWeight()} -> std::convertible_to<common::Weight>;
};

template<typename Edge>
concept HasWeightSetter = requires(Edge& mut_edge, common::Weight weight){
    {mut_edge.setWeight(weight)} -> std::same_as<void>;
};

template<typename Edge>
concept CanHaveShortcuts = requires(const Edge& const_edge, Edge& mut_edge, common::EdgeID id){
    {const_edge.getShortcut()} -> std::same_as<std::optional<std::pair<common::EdgeID, common::EdgeID>>>;

    {const_edge.getShortcutUnsafe()} -> std::same_as<std::pair<common::EdgeID, common::EdgeID>>;

    {mut_edge.setShortcut(id, id)} -> std::same_as<void>;

    {const_edge.isShortcut()} -> std::same_as<bool>;
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

} // namespace concepts
