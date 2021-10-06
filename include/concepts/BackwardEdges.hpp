#pragma once

#include <common/BackwardEdgeView.hpp>
#include <common/BasicGraphTypes.hpp>
#include <concepts/Edges.hpp>
#include <span>

namespace concepts {

// clang-format off
template<typename T>
concept HasBackwardEdges = requires(const T& graph, common::EdgeID id)
{
    requires HasEdges<T>;

	/*
	 * @return a BackwardEdgeView to the edge object associated with the given edge id. The Backwardedgeview contains the null state if such an edge does not exist
	 */
	{graph.getBackwardEdge(id)} noexcept -> std::same_as<common::BackwardEdgeView<typename T::EdgeType>>;
};

// clang-format on
} // namespace concepts
