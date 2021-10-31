#pragma once

#include <concepts/Edges.hpp>
#include <concepts/Nodes.hpp>
#include <optional>

namespace concepts {

// clang-format off

template<typename T>
concept NodesPermutable = requires(T& structure,
								   std::vector<std::size_t> perm,
								   const std::vector<std::size_t>& inv_perm)
{
	//applies a given permutation to the nodes if possible
	//returns true if permutation was applied, false if not
	{structure.applyNodePermutation(perm, inv_perm)} ->std::same_as<bool>;
};
  
template<typename T>
concept EdgesPermutable = requires(T& structure,
								   std::vector<std::size_t> perm,
								   const std::vector<std::size_t>& inv_perm)
{
	//applies a given permutation to the edges if possible
	//returns true if permutation was applied, false if not
	{structure.applyEdgePermutation(perm, inv_perm)} ->std::same_as<bool>;
};
// clang-format on

} // namespace concepts
