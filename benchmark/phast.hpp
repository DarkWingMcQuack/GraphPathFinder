#pragma once

#include <algorithms/distoracle/PHAST.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


class PHASTFixture : public ::benchmark::Fixture
{
public:
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    graphs::OffsetArray<graphs::FMINode<true>, graphs::FMIEdge<true>> graph;
    algorithms::distoracle::PHAST<graphs::FMINode<true>, graphs::FMIEdge<true>> phast;
    std::vector<common::NodeID> srcs;
    std::size_t counter = 0;

    PHASTFixture()
        : graph{parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value()},
          phast{graph}
    {
        graph = algorithms::distoracle::prepareGraphForPHAST(std::move(graph));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes());

        for(std::size_t i = 0; i < 100; i++) {
            srcs.emplace_back(distr(gen));
        }
    }
};


BENCHMARK_DEFINE_F(PHASTFixture, PHASTInitializationBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(algorithms::distoracle::PHAST{graph});
    }
};

BENCHMARK_DEFINE_F(PHASTFixture, PHASTOneToAllDistanceQueryBenchmark)
(benchmark::State& state)
{
    for(auto i : state) {
        benchmark::DoNotOptimize(phast.distancesFrom(srcs[counter]));
        counter = (counter + 1) % 100;
    }
};
