#pragma once

#include <concepts/Edges.hpp>

namespace common {

// can be used to get a view of an edge with source and target inversed
template<class Edge>
requires concepts::HasSource<Edge> && concepts::HasTarget<Edge>
class BackwardEdgeView : public Edge
{
public:
    constexpr auto getTrg() const noexcept
        -> common::NodeID override
    {
        return static_cast<const Edge&>(*this).getSrc();
    }

    constexpr auto getSrc() const noexcept
        -> common::NodeID override
    {
        return static_cast<const Edge&>(*this).getTrg();
    }
};

} // namespace common
