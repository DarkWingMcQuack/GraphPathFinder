#pragma once

#include <common/BackwardEdgeView.hpp>
#include <common/BasicGraphTypes.hpp>
#include <concepts/Edges.hpp>
#include <concepts/Nodes.hpp>
#include <span>

namespace concepts {

// clang-format off
template<typename T>
concept BackwardConnections = requires(const T& graph, common::NodeID src, common::NodeID trg)
{
	/**
	 * checks if an edge between to nodes identified by the two given ids exists
	 * @return true if an edge between the node associated by the first argument 
	 * and the node associated by the second arument exists, false otherwise or if one of the
	 * nodes does not exists
	 */
	{graph.checkIfEdgeExistsBackwardBetween(src, trg)} noexcept -> std::same_as<bool>;

	/**
	 * @return a pointer to the edge id which is associated with the edge between 
	 * the two nodes associated by the given node ids, null if no such edge exists
	 * or if one of the two nodes does not exist.
	 */
	{graph.getBackwardEdgeIDBetween(src, trg)} noexcept -> std::same_as<std::optional<common::EdgeID>>;

	/**
	 * like getEdgeIDBetween but returns a pointer to the acutal edge object instead of the id
	 */
	{graph.getBackwardEdgeBetween(src, trg)} noexcept -> std::same_as<common::BackwardEdgeView<typename T::EdgeType>>;

	/**
	 * @return an span which contains edge ids which are associated with all outgoing edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{graph.getBackwardEdgeIDsOf(src)} noexcept -> std::same_as<std::span<const common::EdgeID>>;
};

template<typename T>
//f is a curried edge comparer which, given a graph returns an function defining an order for edge ids
concept SortableBackwardGraph = requires(T& mut_graph, std::function<std::function<bool(common::EdgeID, common::EdgeID)>(const T&)> f)
{
    requires BackwardConnections<T>;
	requires std::strict_weak_order<decltype(f(mut_graph)),
				        		    common::EdgeID,
								    common::EdgeID>;

	{mut_graph.sortBackwardEdgeIDsAccordingTo(f)};
};
// clang-format on

} // namespace concepts
