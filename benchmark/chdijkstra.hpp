#pragma once

#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>

inline auto CHDijkstraInitialization(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    for(auto _ : state) {
        algorithms::distoracle::CHDijkstra dijk{graph};
        (void)dijk;
    }
}

inline auto CHDijkstraGraphPreparation(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    for(auto _ : state) {
        state.PauseTiming();
        auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
        state.ResumeTiming();
        benchmark::DoNotOptimize(algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph)));
    }
}

inline auto CHDijkstraOneToOne(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes() - 1);
    algorithms::distoracle::CHDijkstra dijk{graph};

    while(state.KeepRunning()) {
        state.PauseTiming();
        common::NodeID s{distr(gen)};
        common::NodeID t{distr(gen)};
        state.ResumeTiming();

        benchmark::DoNotOptimize(dijk.distanceBetween(s, t));
    }
}
