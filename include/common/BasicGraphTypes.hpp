#pragma once

#include <NamedType/named_type.hpp>
#include <charconv>
#include <concepts>
#include <cstdint>
#include <limits>
#include <string_view>

namespace common {

// clang-format off
using NodeID     = fluent::NamedType<std::size_t, struct NodeIDTag, fluent::Arithmetic>;
using EdgeID     = fluent::NamedType<std::size_t, struct EdgeIDTag, fluent::Arithmetic>;
using NodeLevel  = fluent::NamedType<std::size_t, struct NodeLevelTag, fluent::Arithmetic>;
using Elevation  = fluent::NamedType<std::size_t, struct ElevationTag, fluent::Arithmetic>;
using Speed      = fluent::NamedType<std::int_fast64_t, struct SpeedTag, fluent::Arithmetic>;
using Type       = fluent::NamedType<std::int_fast64_t, struct TypeTag, fluent::Arithmetic>;
using Weight     = fluent::NamedType<std::int_fast64_t, struct WeightTag, fluent::Arithmetic>;
using Latitude   = fluent::NamedType<double, struct LatitudeTag, fluent::Arithmetic>;
using Longitude  = fluent::NamedType<double, struct LongitudeTag, fluent::Arithmetic>;
// clang-format on

constexpr const static inline auto INFINITY_WEIGHT = Weight{std::numeric_limits<std::int_fast64_t>::max()};
constexpr const static inline auto UNKNOWN_NODE_ID = NodeID{std::numeric_limits<std::size_t>::max()};
constexpr const static inline auto UNKNOWN_EDGE_ID = EdgeID{std::numeric_limits<std::size_t>::max()};
constexpr const static inline auto MAX_LEVEL = NodeLevel{std::numeric_limits<std::size_t>::max()};

} // namespace common
