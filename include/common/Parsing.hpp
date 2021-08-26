#pragma once

#include <charconv>
#include <common/BasicGraphTypes.hpp>
#include <concepts>
#include <optional>
#include <string_view>


namespace common {

// clang-format off
template<class T>
auto to(const std::string_view& input) noexcept
    -> std::optional<T>
requires
  (std::is_same_v<T, NodeID>    ||
   std::is_same_v<T, EdgeID>    ||
   std::is_same_v<T, Weight>    ||
   std::is_same_v<T, NodeLevel>)
  && std::is_integral_v<typename T::UnderlyingType>

// clang-format on
{
    typename T::UnderlyingType out;

    const auto start = std::min(input.find_first_not_of(" \t"), input.size());

    const auto result = std::from_chars(input.data() + start,
                                        input.data() + input.size(),
                                        out);

    if(result.ec == std::errc::invalid_argument
       or result.ec == std::errc::result_out_of_range) {
        return std::nullopt;
    }

    return T{out};
}


// clang-format off
template<class T>
auto to(const std::string_view& input) noexcept
    -> std::optional<T>
requires
  (std::is_same_v<T, Longitude>    ||
   std::is_same_v<T, Latitude>)
  && std::is_floating_point_v<typename T::UnderlyingType>
// clang-format on
{
    char* end;
    typename T::UnderlyingType d;

    const auto start = std::min(input.find_first_not_of(" \t"), input.size());

    if constexpr(std::is_same_v<typename T::UnderlyingType, double>) {
        d = std::strtod(input.data() + start, &end);
    } else if constexpr(std::is_same_v<typename T::UnderlyingType, float>) {
        d = std::strtof(input.data() + start, &end);
    } else if constexpr(std::is_same_v<typename T::UnderlyingType, long double>) {
        d = std::strtold(input.data() + start, &end);
    } else {
        static_assert(true, "parsing not supported for the given type");
    }


    if(input.data() == end) {
        return std::nullopt;
    }

    return T{d};
}

} // namespace common
