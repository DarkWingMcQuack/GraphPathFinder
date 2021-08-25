// #include <NamedType/named_type.hpp>
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

struct A
{
protected:
    int i;
    int j;

public:
    A(int i, int j)
        : i(i), j(j) {}



    auto bla() const noexcept
        -> void
    {
        fmt::print("i: {}, j: {}\n", i, j);
    }

    auto getI() const noexcept
        -> int
    {
        return i;
    }

    auto getJ() const noexcept
        -> int
    {
        return j;
    }
};

template<class T>
class Inverse : public T
{
public:
    auto bla() const noexcept
        -> void
    {
        fmt::print("i: {}, j: {}\n", static_cast<const T*>(this)->getJ(), static_cast<const T*>(this)->getI());
    }
};


auto main() -> int
{
    const auto* a = new A{1, 10};
    a->bla();

    const auto* b = static_cast<const Inverse<A>*>(a);
    b->bla();
}
