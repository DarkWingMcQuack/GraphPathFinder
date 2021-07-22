#pragma once

#include <concepts/structures/Edges.hpp>
#include <concepts/structures/Nodes.hpp>
#include <span>

namespace concepts::structures {

// clang-format off
template<typename T>
concept BackwardGraph = requires(const T& graph, NodeID src, NodeID trg)
{
    requires Nodes<T>;
    requires Edges<T>;

	/**
	 * @return a pointer to the edge id which is associated with the edge between 
	 * the two nodes associated by the given node ids, null if no such edge exists
	 * or if one of the two nodes does not exist.
	 */
	{graph.getBackwardEdgeIDBetween(src, trg)} noexcept -> std::same_as<const EdgeID*>;

	/**
	 * like getEdgeIDBetween but returns a pointer to the acutal edge object instead of the id
	 */
	{graph.getBackwardEdgeBetween(src, trg)} noexcept -> std::same_as<const typename T::EdgeType*>;


	/*Ü
	 * @return an span which contains edge ids which are associated with all outgoing edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{graph.getBackwardEdgeIDsOf(T::NodeIDType)} noexcept -> std::same_as<std::span<const EdgeID>>;
};
// clang-format on

} // namespace concepts::structures
