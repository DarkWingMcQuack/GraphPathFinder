#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class LevelBase
{
public:
    constexpr LevelBase(common::NodeLevel lvl) noexcept
        : level_(lvl) {}

    constexpr LevelBase(LevelBase&&) noexcept = default;
    constexpr LevelBase(const LevelBase&) noexcept = default;

    constexpr auto operator=(const LevelBase&) noexcept
        -> LevelBase& = default;
    constexpr auto operator=(LevelBase&&) noexcept
        -> LevelBase& = default;

    constexpr auto operator==(const LevelBase&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const LevelBase&) const noexcept
        -> bool = default;

    constexpr auto getLvl() const noexcept
        -> common::NodeLevel
    {
        return level_;
    }

    constexpr auto setLvl(common::NodeLevel lvl) noexcept
        -> void
    {
        level_ = lvl;
    }

private:
    common::NodeLevel level_;
};

} // namespace graphs
