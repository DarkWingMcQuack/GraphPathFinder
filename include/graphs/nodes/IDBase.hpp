#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class IDBase
{
public:
    constexpr IDBase(common::NodeID id) noexcept
        : id_(id) {}

    constexpr auto getID() const noexcept
        -> common::NodeID
    {
        return id_;
    }

private:
    common::NodeID id_;
};

} // namespace graphs
