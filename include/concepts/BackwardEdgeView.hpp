#pragma once

#include <concepts/Edges.hpp>

namespace concepts::structures {

// can be used to get a view of an edge with source and target inversed
template<class Edge>
requires HasSource<Edge> && HasTarget<Edge>
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

} // namespace concepts::structures
