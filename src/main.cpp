#include <NamedType/named_type.hpp>
// #include <concepts/algorithms/DistanceOracle.hpp>
#include <bit>
// #include <concepts/algorithms/PathOracle.hpp>
// #include <concepts/structures/BackwardGraph.hpp>
// #include <concepts/structures/EdgeWeights.hpp>
// #include <concepts/structures/Edges.hpp>
// #include <concepts/structures/ForwardGraph.hpp>
// #include <concepts/structures/NodeLevels.hpp>
// #include <concepts/structures/Nodes.hpp>
// #include <concepts/structures/Path.hpp>
#include <fmt/core.h>
// #include <impl/graphs/ForwardOffsetArray.hpp>


using Foo = fluent::NamedType<std::size_t, struct FooTag, fluent::Arithmetic>;

auto main() -> int
{
    Foo{1} == Foo{2};
}
