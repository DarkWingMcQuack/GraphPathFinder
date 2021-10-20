#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class SrcBase
{
public:
    constexpr SrcBase(common::NodeID src) noexcept
        : src_(src) {}

    constexpr auto getSrc() const noexcept
        -> common::NodeID
    {
        return src_;
    }

    constexpr auto setSrc(common::NodeID src) noexcept
        -> void
    {
        src_ = src;
    }

private:
    common::NodeID src_;
};

} // namespace graphs
