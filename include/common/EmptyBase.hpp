#pragma once

namespace common {


//empty structs which cane be used for conditional inheritance with std::conditional
struct EmptyBase1
{
    constexpr auto operator==(const EmptyBase1&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const EmptyBase1&) const noexcept
        -> bool = default;
};

struct EmptyBase2
{
    constexpr auto operator==(const EmptyBase2&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const EmptyBase2&) const noexcept
        -> bool = default;
};

struct EmptyBase3
{
    constexpr auto operator==(const EmptyBase3&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const EmptyBase3&) const noexcept
        -> bool = default;
};

struct EmptyBase4
{
    constexpr auto operator==(const EmptyBase4&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const EmptyBase4&) const noexcept
        -> bool = default;
};

} // namespace common
