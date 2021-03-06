#pragma once

#include <common/BasicGraphTypes.hpp>

namespace graphs {

class ShortcutBase
{
public:
    constexpr ShortcutBase(common::EdgeID first,
                           common::EdgeID second) noexcept
        : shortcut_(std::pair{first, second}) {}

    constexpr ShortcutBase() noexcept = default;

    constexpr auto getShortcut() const noexcept
        -> std::optional<std::pair<common::EdgeID, common::EdgeID>>
    {
        return shortcut_;
    }

    constexpr auto getShortcutUnsafe() const noexcept
        -> std::pair<common::EdgeID, common::EdgeID>
    {
        return shortcut_.value();
    }

    //TODO: it seems that std::optional<T>::emplace is not constexpr in gcc 10
    //according to the standard it should be in C++20
    auto setShortcut(common::EdgeID first, common::EdgeID second) noexcept
        -> void
    {
        shortcut_.emplace(first, second);
    }

    constexpr auto isShortcut() const noexcept
        -> bool
    {
        return shortcut_.has_value();
    }

private:
    std::optional<std::pair<common::EdgeID, common::EdgeID>> shortcut_;
};

} // namespace graphs
