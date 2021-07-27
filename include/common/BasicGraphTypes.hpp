#pragma once

#include <NamedType/named_type.hpp>
#include <concepts>

namespace common {

// clang-format off
using NodeID     = fluent::NamedType<std::size_t, struct NodeIDTag>;
using EdgeID     = fluent::NamedType<std::size_t, struct EdgeIDTag>;
using EdgeWeight = fluent::NamedType<std::int_fast64_t, struct EdgeWeightTag>;
using NodeLevel  = fluent::NamedType<std::int_fast64_t, struct NodeLevelTag>;
// clang-format on

} // namespace common
