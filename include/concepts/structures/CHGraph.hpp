#pragma once

#include <concepts/structures/BackwardGraph.hpp>
#include <concepts/structures/EdgeWeights.hpp>
#include <concepts/structures/ForwardGraph.hpp>
#include <concepts/structures/NodeLevels.hpp>
#include <span>

namespace concepts::structures {

template<typename T>
concept CHGraph = ForwardGraph<T> && BackwardGraph<T> && ReadableEdgeWeights<T> && ReadableNodeLevels<T>;

}
