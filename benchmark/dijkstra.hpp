#pragma once

#include <algorithms/distoracle/dijkstra/Dijkstra.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


class DijkstraFixture : public ::benchmark::Fixture
{
public:
    const char* const example_graph = "../data/stgtregbz.txt";
    graphs::OffsetArray<graphs::FMINode<false>, graphs::FMIEdge<false>> graph;
    algorithms::distoracle::Dijkstra<graphs::OffsetArray<graphs::FMINode<false>, graphs::FMIEdge<false>>> dijk;
    std::vector<common::NodeID> srcs;
    std::vector<common::NodeID> trgs;
    std::size_t counter = 0;

    DijkstraFixture()
        : graph{parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph).value()},
          dijk{graph}
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes());

        for(std::size_t i = 0; i < 100; i++) {
            srcs.emplace_back(distr(gen));
            trgs.emplace_back(distr(gen));
        }
    }
};


BENCHMARK_DEFINE_F(DijkstraFixture, DijkstraInitializationBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(algorithms::distoracle::Dijkstra{graph});
    }
};

BENCHMARK_DEFINE_F(DijkstraFixture, DijkstraOneToOneDistanceQueryBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(dijk.distanceBetween(srcs[counter], trgs[counter]));
        counter = (counter + 1) % 100;
    }
};
