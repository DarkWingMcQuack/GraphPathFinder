#pragma once

#include <concepts/BackwardGraph.hpp>
#include <concepts/EdgeWeights.hpp>
#include <concepts/ForwardGraph.hpp>
#include <concepts/NodeLevels.hpp>
#include <span>

namespace concepts::structures {

template<typename T>
concept CHGraph = ForwardGraph<T> && BackwardGraph<T> && ReadableEdgeWeights<T> && ReadableNodeLevels<T>;

}
