#pragma once

#include <concept/Graph.hpp>
#include <concepts>
#include <optional>
#include <span>

namespace concepts {

// clang-format off
template<typename T>
concept BackwardEdgeInfoGraph = requires(const T& graph)
{
    requires Graph<T>;

	/*
	 * @return an span which contains edge ids which are associated with all incomming edges of a node associated
	 * with the given node id, the span is empty if no such node exists or if the node does not have any outgoing edges
	 */
	{graph.getIncomingEdgeIDsOf(T::NodeIDType)} noexcept -> std::same_as<std::span<const typename T::EdgeIDType>>;
};
// clang-format on

} // namespace concepts
