#pragma once

#include <span>

// clang-format off
template<typename T>
concept Graph = requires(const T& graph)
{
    typename T::NodeType;   // require node type
    typename T::EdgeType;   // require edge type
    typename T::NodeIDType; // require node id type
    typename T::EdgeIDType; // require edge id type

	/*
	 * checks if a node with the given ID exists
	 * @return true if a node with the given ID exists, false otherwise
	 */
	{graph.nodeExists(T::NodeIDType)} noexcept                       -> bool;

	/*
	 * returns a pointer to the node class associated with the
	 * given ID
	 * @return a non owning pointer to the node object associated with the given id,
	 * null if no such object exists
	 */
	{graph.getNode(T::NodeIDType)} noexcept                          -> const T::NodeType*;
	
	/*
	 * checks if an edge between to nodes identified by the two given ids exists
	 * @return true if an edge between the node associated by the first argument 
	 * and the node associated by the second arument exists, false otherwise or if one of the
	 * nodes does not exists
	 */
	{graph.edgeBetweenExists(T::NodeIDType, T::NodeIDType)} noexcept -> bool;

	/*
	 * @return a pointer to the edge id which is associated with the edge between 
	 * the two nodes associated by the given node ids, null if no such edge exists
	 * or if one of the two nodes does not exist.
	 */
	{graph.getEdgeIDBetween(T::NodeIDType, T::NodeIDType)} noexcept  -> const T::EdgeIDType*;

	/*
	 * like getEdgeIDBetween but returns a pointer to the acutal edge object instead of the id
	 */
	{graph.getEdgeBetween(T::NodeIDType, T::NodeIDType)} noexcept    -> const T::EdgeType*;

	/*
	 * check if an edge associated with the given edge id exists
	 * @return true if an edge associated with the given edge id exists, false otherwise
	 */
	{graph.edgeExists(T::EdgeIDType)} noexcept -> bool

	/*
	 * @return a pointer to the edge object associated with the given edge id, null if no such edge exists
	 */
	{graph.getEdge(T::EdgeIDType)} noexcept    -> const T::EdgeType*;

	/*
	 * @return an span which contains edge ids which are associated with all outgoing edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{graph.getOutgoingEdgeIDsOf(T::NodeIDType)} noexcept -> std::span<const T::EdgeIDType>;

	
	/*
	 * @return the number of edges
	 */
	{graph.numberOfNodes()} noexcept -> std::size_t;

	/*
	 * @return the number of nodes
	 */
	{graph.numberOfEdges()} noexcept -> std::size_t;
};

// clang-format on
