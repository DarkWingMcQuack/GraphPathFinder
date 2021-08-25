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
  std::is_same_v<T, NodeID>    ||
  std::is_same_v<T, EdgeID>    ||
  std::is_same_v<T, Weight>    ||
  std::is_same_v<T, NodeLevel>
//clang-format on
{
    typename T::UnderlyingType out;
    const auto result = std::from_chars(input.data(),
                                        input.data() + input.size(),
                                        out);

    if(result.ec == std::errc::invalid_argument
       or result.ec == std::errc::result_out_of_range) {
        return std::nullopt;
    }

    return T{out};
}


template<class T>
auto to(const std::string_view& input) noexcept
    -> std::optional<T>
requires std::is_same_v<typename T::UnderlyingType, double>
//clang-format on
{
    char* end;

    double d = std::strtod(input.data(), &end);

    if(input.data() == end) {
	  return std::nullopt;
    }

	return T{d};
}

  
} // namespace common
