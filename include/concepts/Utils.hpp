#pragma once

#include <concepts/structures/EdgeWeights.hpp>
#include <concepts/structures/Nodes.hpp>
#include <optional>
#include <vector>

namespace concepts {

template<class, template<class...> class>
inline constexpr auto is_specialization = false;
template<template<class...> class T, class... Args>
inline constexpr auto is_specialization<T<Args...>, T> = true;

template<class T>
concept is_vector = is_specialization<T, std::vector>;

template<class T>
concept is_optional = is_specialization<T, std::optional>;

// clang-format off


// clang-format on

} // namespace concepts
