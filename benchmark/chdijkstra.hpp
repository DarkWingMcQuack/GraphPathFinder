#pragma once

#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


class CHDijkstraFixture : public ::benchmark::Fixture
{
public:
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    graphs::OffsetArray<graphs::FMINode<true>, graphs::FMIEdge<true>> graph;
    algorithms::distoracle::CHDijkstra<graphs::OffsetArray<graphs::FMINode<true>, graphs::FMIEdge<true>>> dijk;
    std::vector<common::NodeID> srcs;
    std::vector<common::NodeID> trgs;
    std::size_t counter = 0;

    CHDijkstraFixture()
        : graph{parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value()},
          dijk{graph}
    {
        graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes());

        for(std::size_t i = 0; i < 100; i++) {
            srcs.emplace_back(distr(gen));
            trgs.emplace_back(distr(gen));
        }
    }
};


BENCHMARK_DEFINE_F(CHDijkstraFixture, CHDijkstraInitializationBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(algorithms::distoracle::CHDijkstra{graph});
    }
};

BENCHMARK_DEFINE_F(CHDijkstraFixture, CHDijkstraOneToOneDistanceQueryBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(dijk.distanceBetween(srcs[counter], trgs[counter]));
        counter = (counter + 1) % 100;
    }
};
