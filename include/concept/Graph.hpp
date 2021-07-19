#pragma once

#include <span>

// clang-format off
template<typename T>
concept Graph = requires(const T& graph)
{
    typename T::NodeType; // require node type
    typename T::EdgeType; // require node type
    typename T::NodeIDType; // require node type
    typename T::EdgeIDType; // require node type

	{graph.nodeExists(T::NodeIDType)}                       -> bool;
	{graph.getNode(T::NodeIDType)}                          -> const T::NodeType*;
	{graph.edgeBetweenExists(T::NodeIDType, T::NodeIDType)} -> bool;

	{graph.edgeExists(T::EdgeIDType)} -> bool
	{graph.getEdge(T::EdgeIDType)}    -> const T::EdgeType*;

	{graph.getEdgeIDsOf(T::NodeIDType)} -> std::span<const EdgeIDType>;
};

// clang-format on
