#include "algorithms/distoracle/PHAST.hpp"
#include "algorithms/distoracle/ch/CHDijkstra.hpp"
#include "algorithms/distoracle/dijkstra/Dijkstra.hpp"
#include "algorithms/distoracle/hublabels/HubLabelCalculator.hpp"
#include "graphs/edges/FMIEdge.hpp"
#include "graphs/nodes/FMINode.hpp"
#include "parsing/offsetarray/Parser.hpp"
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <chrono>
#include <common/BasicGraphTypes.hpp>
#include <random>

using common::NodeID;


const auto* const ch_graph = "../data/ch-stgtregbz.txt";
const auto* const simple_graph = "../data/stgtregbz.txt";

class Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using MilliSecond = std::chrono::duration<double, std::milli>;

public:
    Timer()
        : beg_(Clock::now()) {}

    auto reset()
        -> void
    {
        beg_ = Clock::now();
    }
    auto elapsed() const
        -> double
    {
        return std::chrono::duration_cast<MilliSecond>(Clock::now()
                                                       - beg_)
            .count();
    }

private:
    std::chrono::time_point<Clock> beg_;
};


template<class Tp>
inline __attribute__((always_inline)) void DoNotOptimize(Tp const& value)
{
    asm volatile(""
                 :
                 : "r,m"(value)
                 : "memory");
}

template<class Tp>
inline __attribute__((always_inline)) void DoNotOptimize(Tp& value)
{
#if defined(__clang__)
    asm volatile(""
                 : "+r,m"(value)
                 :
                 : "memory");
#else
    asm volatile(""
                 : "+m,r"(value)
                 :
                 : "memory");
#endif
}


template<class Oracle>
auto benchmarkOneToOne(const std::vector<common::NodeID>& srcs,
                       const std::vector<NodeID>& trgs,
                       Oracle& o) noexcept
    -> double
{
    Timer timer;
    for(std::size_t i = 0; i < srcs.size(); i++) {
        DoNotOptimize(o.distanceBetween(srcs[i], trgs[i]));
    }

    return timer.elapsed() / static_cast<double>(srcs.size());
}

template<class Oracle>
auto benchmarkOneToMany(const std::vector<common::NodeID>& srcs,
                        Oracle& o) noexcept
    -> double
{
    Timer timer;
    for(const auto src : srcs) {
        DoNotOptimize(o.distancesFrom(src));
    }

    return timer.elapsed() / static_cast<double>(srcs.size());
}

template<class Graph>
auto benchmarkHublabelCreation(const Graph& g) noexcept
    -> std::pair<double, algorithms::distoracle::HubLabelLookup>
{
    Timer timer;
    algorithms::distoracle::HubLabelCalculator creator{g};
    auto lookup = creator.constructHubLabelLookupInParallel();
    auto elapsed = timer.elapsed();

    return std::pair{elapsed, std::move(lookup)};
}

auto createPHASTGraph(std::string_view path)
{
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(path).value();
    return algorithms::distoracle::prepareGraphForPHAST(std::move(graph));
}

auto createHubLabelGraph(std::string_view path)
{
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(path).value();
    return algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph));
}

auto createCHDijkstraGraph(std::string_view path)
{
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(path).value();
    return algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));
}

auto createDijkstraGraph(std::string_view path)
{
    return parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(path).value();
}

auto createData(std::size_t n, std::size_t number_of_nodes)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, number_of_nodes);

    std::vector<common::NodeID> srcs;
    std::vector<common::NodeID> trgs;
    for(std::size_t i = 0; i < n; i++) {
        srcs.emplace_back(distr(gen));
        trgs.emplace_back(distr(gen));
    }

    return std::pair{std::move(srcs), std::move(trgs)};
}

auto main(int argc, char* argv[])
    -> int
{
    fmt::print("-------------------One to One-------------------\n");
    auto dijkstra = [] {
        auto graph = createDijkstraGraph(simple_graph);
        algorithms::distoracle::Dijkstra dijk{graph};
        auto [srcs, trgs] = createData(100, graph.numberOfNodes());
        return benchmarkOneToOne(srcs, trgs, dijk);
    }();
    fmt::print("Dijkstra: {} ms\n", dijkstra);

    auto ch_dijkstra = [] {
        auto graph = createCHDijkstraGraph(ch_graph);
        algorithms::distoracle::CHDijkstra dijk{graph};
        auto [srcs, trgs] = createData(1000, graph.numberOfNodes());
        return benchmarkOneToOne(srcs, trgs, dijk);
    }();
    fmt::print("CH-Dijkstra: {} μs\n", ch_dijkstra*1000.);

    auto [lookup_creation, lookup] = [] {
        auto graph = createHubLabelGraph(ch_graph);
        return benchmarkHublabelCreation(graph);
    }();
    fmt::print("HL Construction: {} s\n", lookup_creation / 1000.);

    auto hublabels = [lookup = std::move(lookup)] {
        auto [srcs, trgs] = createData(1000, lookup.numberOfNodes());
        return benchmarkOneToOne(srcs, trgs, lookup);
    }();
    fmt::print("HL: {} μs\n", hublabels * 1000.);

    fmt::print("-------------------One to All-------------------\n");
    auto phast = [] {
        auto graph = createPHASTGraph(ch_graph);
        algorithms::distoracle::PHAST phast{graph};
        auto [srcs, trgs] = createData(100, graph.numberOfNodes());
        return benchmarkOneToMany(srcs, phast);
    }();
    fmt::print("PHAST: {} ms\n", phast);

    auto dijkstra_one_to_all = [] {
        auto graph = createDijkstraGraph(simple_graph);
        algorithms::distoracle::Dijkstra dijkstra{graph};
        auto [srcs, trgs] = createData(100, graph.numberOfNodes());
        return benchmarkOneToMany(srcs, dijkstra);
    }();
    fmt::print("Dijkstra: {} ms\n", dijkstra_one_to_all);

    return 0;
}
