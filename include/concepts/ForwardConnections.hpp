#pragma once

#include <common/BasicGraphTypes.hpp>
#include <concepts/Edges.hpp>
#include <concepts/Nodes.hpp>
#include <span>

namespace concepts {

// clang-format off
template<typename T>
concept ForwardConnections = requires(const T& graph, T& mut_graph, common::NodeID src, common::NodeID trg)
{
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


template<typename T>
//f is a curried edge comparer which, given a graph returns an function defining an order for edge ids
concept SortableForwardConnections = requires(T& mut_graph, std::function<std::function<bool(common::EdgeID, common::EdgeID)>(const T&)> f)
{
    requires ForwardConnections<T>;
	requires std::strict_weak_order<decltype(f(mut_graph)),
				        		    common::EdgeID,
								    common::EdgeID>;

	{mut_graph.sortForwardEdgeIDsAccordingTo(f)};
};

template<typename T>
concept DeletableForwardConnections = requires(T& mut_graph, std::function<std::function<bool(common::EdgeID)>(const T&)> f)
{
    requires ForwardConnections<T>;

	{mut_graph.deleteForwardEdgesIDsIf(f)};
};
// clang-format on

} // namespace concepts
