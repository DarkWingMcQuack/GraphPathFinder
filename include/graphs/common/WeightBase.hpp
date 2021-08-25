#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class WeightBase
{
public:
    constexpr WeightBase(common::Weight weight) noexcept
        : weight_(weight) {}

    constexpr auto getWeight() const noexcept
        -> common::Weight
    {
        return weight_;
    }

private:
    common::Weight weight_;
};

} // namespace graphs
