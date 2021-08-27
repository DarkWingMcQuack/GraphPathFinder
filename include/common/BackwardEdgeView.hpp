#pragma once

#include <concepts/Edges.hpp>

namespace common {

// can be used to get a view of an edge with source and target inversed
template<class Edge>
requires concepts::HasSource<Edge> && concepts::HasTarget<Edge>
class BackwardEdgeView
{
public:
    constexpr BackwardEdgeView(const Edge* e)
        : e_(e) {}

    constexpr auto getTrg() const noexcept
        -> common::NodeID
    {
        return e_->getSrc();
    }

    constexpr auto getSrc() const noexcept
        -> common::NodeID
    {
        return e_->getTrg();
    }

    constexpr auto isNullPtr() const noexcept
        -> bool
    {
        return e_ == nullptr;
    }

    constexpr auto getForwardWeight() const noexcept
        -> bool requires concepts::HasWeight<Edge>
    {
        return e_->getWeight();
    }

    constexpr explicit operator bool() const noexcept
    {
        return e_ != nullptr;
    }

    constexpr auto operator->() const noexcept
        -> const BackwardEdgeView<Edge>*
    {
        return this;
    }

private:
    const Edge* e_;
};

} // namespace common
