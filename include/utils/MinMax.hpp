#pragma once

#include <utility>

namespace util {

template<class Head0, class Head1, class... Tail>
constexpr auto min(Head0&& head0, Head1&& head1, Tail&&... tail) noexcept
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 < head1 ? head0 : head1;
    } else {
        return min(
            min(std::forward<Head0>(head0),
                std::forward<Head1>(head1)),
            std::forward<Tail>(tail)...);
    }
}

template<class Head0, class Head1, class... Tail>
constexpr auto max(Head0&& head0, Head1&& head1, Tail&&... tail) noexcept
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 > head1 ? head0 : head1;
    } else {
        return max(
            max(std::forward<Head0>(head0),
                std::forward<Head1>(head1)),
            std::forward<Tail>(tail)...);
    }
}
} // namespace util
