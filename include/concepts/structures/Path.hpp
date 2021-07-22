#pragma once

#include <concepts/Utils.hpp>
#include <concepts/structures/EdgeWeights.hpp>
#include <concepts/structures/Nodes.hpp>
#include <optional>
#include <span>

namespace concepts::structures {

// clang-format off

template<typename P>
concept Path = requires(const P& path)
{

	/**
	 * @returns the number of nodes this path represents
	 */
    {path.numberOfNodes()} noexcept -> std::same_as<std::size_t>;

	/**
	 * @returns the number of edges this path represents
	 */
    {path.numberOfEdges()} noexcept -> std::same_as<std::size_t>;

	/**
	 * @returns the source of the path if available if the path is empty this returns std::nullopt
	 * if the path has only one node, path.source() and path.target() musst return the same result
	 */
	{path.source()} noexcept -> std::same_as<std::optional<NodeID>>;

	/**
	 * @returns the target of the path if available if the path is empty this returns std::nullopt
	 * if the path has only one node, path.source() and path.target() musst return the same result
	 */
	{path.target()} noexcept -> std::same_as<std::optional<NodeID>>;

	/**
	 * this method can be used to access the total cost of the path
	 * @returns the sum of the weights of the edges of the path. If the path is from a graph which 
	 * does not have any edgeweights it returns the number of edges @see path.numberOfEdges
	 */
	{path.cost()} noexcept -> EdgeWeight;

	//TODO: paths should have the ability to iterate over their edge ids as well as their node ids
	// {path.nodeIDs().begin()} -> ???
	// {path.nodeIDs().end()}   -> ???

	// {path.edgeIDs().begin()} -> ???
	// {path.edgeIDs().end()}   -> ???
};

  
template<typename T>
concept PathOpt = requires
{
    typename T::value_type;
	requires Path<typename T::value_type>;
	requires is_optional<T>;
};

// clang-format on

} // namespace concepts::structures
