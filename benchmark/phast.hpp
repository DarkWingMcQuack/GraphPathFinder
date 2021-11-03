#pragma once

#include <algorithms/distoracle/PHAST.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


inline auto PHASTGraphPreparation(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    for(auto _ : state) {
        state.PauseTiming();
        auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
        state.ResumeTiming();
        benchmark::DoNotOptimize(algorithms::distoracle::prepareGraphForPHAST(std::move(graph)));
    }
}

inline auto PHASTInitialization(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    graph = algorithms::distoracle::prepareGraphForPHAST(std::move(graph));

    while(state.KeepRunning()) {
        algorithms::distoracle::PHAST phast{graph};
        (void)phast;
    }
}

inline auto PHASTOneToAll(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    graph = algorithms::distoracle::prepareGraphForPHAST(std::move(graph));
    algorithms::distoracle::PHAST phast{graph};


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes() - 1);

    while(state.KeepRunning()) {
        state.PauseTiming();
        common::NodeID s{distr(gen)};
        state.ResumeTiming();

        benchmark::DoNotOptimize(phast.distancesFrom(s));
    }
}
