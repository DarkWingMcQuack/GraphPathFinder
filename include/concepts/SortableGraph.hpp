#pragma once

#include <concepts/BackwardGraph.hpp>
#include <concepts/Edges.hpp>
#include <concepts/ForwardGraph.hpp>
#include <concepts/Nodes.hpp>
#include <optional>

namespace concepts {

// clang-format off

template<typename Graph>
//f is a curried node comparer which when called with a graph returns a function defining an ordering on node ids
concept NodesSortable = requires(Graph& graph, std::function<std::function<bool(common::NodeID, common::NodeID)>(const Graph&)> f)
{
    requires HasNodes<Graph>;
	requires std::strict_weak_order<decltype(f(graph)),
				        		    common::NodeID,
								    common::NodeID>;

	//sorts the nodes according to the function generated from calling f with graph g
	//returns the permutation vector of the ordering
	{graph.sortNodesAccordingTo(f)} -> std::same_as<std::vector<std::size_t>>;
};

  
template<typename Graph>
//f is a curried edge comparer which when called with a graph returns a function defining an ordering on edge ids
concept EdgesSortable = requires(Graph& graph, std::function<std::function<bool(common::EdgeID, common::EdgeID)>(const Graph&)> f)
{
    requires HasEdges<Graph>;
	requires std::strict_weak_order<decltype(f(graph)),
				        		    common::EdgeID,
								    common::EdgeID>;

	//sorts the nodes according to the function generated from calling f with graph g
	//returns the permutation vector of the ordering
	{graph.sortEdgesAccordingTo(f)} -> std::same_as<std::vector<std::size_t>>;
};
// clang-format on

} // namespace concepts
