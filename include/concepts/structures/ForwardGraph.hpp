#pragma once

#include <common/BasicGraphTypes.hpp>
#include <concepts/structures/Edges.hpp>
#include <concepts/structures/Nodes.hpp>
#include <span>

namespace concepts::structures {

// clang-format off
template<typename T>
concept ForwardGraph = requires(const T& graph, T& mut_graph, common::NodeID src, common::NodeID trg)
{
    requires HasNodes<T>;
    requires HasEdges<T>;

	/**
	 * checks if an edge between to nodes identified by the two given ids exists
	 * @return true if an edge between the node associated by the first argument 
	 * and the node associated by the second arument exists, false otherwise or if one of the
	 * nodes does not exists
	 */
	{graph.checkIfEdgeExistsBetween(src, trg)} noexcept -> std::same_as<bool>;

	/**
	 * @return a pointer to the edge id which is associated with the edge between 
	 * the two nodes associated by the given node ids, null if no such edge exists
	 * or if one of the two nodes does not exist.
	 */
	{graph.getForwardEdgeIDBetween(src, trg)} noexcept -> std::same_as<std::optional<common::EdgeID>>;

	/**
	 * like getEdgeIDBetween but returns a pointer to the acutal edge object instead of the id
	 */
	{graph.getForwardEdgeBetween(src, trg)} noexcept -> std::same_as<const typename T::EdgeType*>;


	/**
	 * @return an span which contains edge ids which are associated with all outgoing edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{graph.getForwardEdgeIDsOf(src)} noexcept -> std::same_as<std::span<const common::EdgeID>>;

	/**
	 * @return an span which contains edge ids which are associated with all outgoing edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{mut_graph.getForwardEdgeIDsOf(src)} noexcept -> std::same_as<std::span<common::EdgeID>>;
};
// clang-format on

} // namespace concepts::structures
