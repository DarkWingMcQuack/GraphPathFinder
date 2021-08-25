#pragma once

#include <optional>
#include <string_view>

namespace concepts {

// clang-format off
template<typename T>
concept Parseable = requires(std::string_view str){
  {T::parse(str)} -> std::same_as<std::optional<T>>;
};

template<typename T>
concept UnsafeParseable = requires(std::string_view str){
  {T::parseUnsafe(str)} -> std::same_as<T>;
};
// clang-format on

} // namespace concepts
