#pragma once

#include <string_view>

namespace concepts {

// clang-format off
template<typename T>
concept ParseableFromLine = requires(std::string_view line){
    {T::parse(line)} -> std::same_as<T>;
};
// clang-format on

} // namespace concepts
