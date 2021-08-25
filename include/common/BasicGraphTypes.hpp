#pragma once

#include <NamedType/named_type.hpp>
#include <charconv>
#include <concepts>
#include <string_view>

namespace common {

// clang-format off
using NodeID     = fluent::NamedType<std::size_t, struct NodeIDTag>;
using EdgeID     = fluent::NamedType<std::size_t, struct EdgeIDTag>;
using Weight     = fluent::NamedType<std::int_fast64_t, struct WeightTag>;
using NodeLevel  = fluent::NamedType<std::int_fast64_t, struct NodeLevelTag>;
using Latitude   = fluent::NamedType<double, struct LatitudeTag>;
using Longitude  = fluent::NamedType<double, struct LongitudeTag>;
// clang-format on

} // namespace common
