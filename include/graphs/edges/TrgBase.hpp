#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class TrgBase
{
public:
    constexpr TrgBase(common::NodeID trg) noexcept
        : trg_(trg) {}

    constexpr auto getTrg() const noexcept
        -> common::NodeID
    {
        return trg_;
    }

private:
    common::NodeID trg_;
};

} // namespace graphs
