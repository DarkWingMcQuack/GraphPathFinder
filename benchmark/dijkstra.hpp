#pragma once

#include <algorithms/distoracle/dijkstra/Dijkstra.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


inline auto DijkstraInitialization(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph).value();
    for(auto _ : state) {
        algorithms::distoracle::Dijkstra dijk{graph};
        (void)dijk;
    }
}

inline auto DijkstraOneToOne(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph).value();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes() - 1);
    algorithms::distoracle::Dijkstra dijk{graph};

    while(state.KeepRunning()) {
        state.PauseTiming();
        common::NodeID s{distr(gen)};
        common::NodeID t{distr(gen)};
        state.ResumeTiming();

        benchmark::DoNotOptimize(dijk.distanceBetween(s, t));
    }
}

inline auto DijkstraOneToAll(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph).value();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, graph.numberOfNodes() - 1);
    algorithms::distoracle::Dijkstra dijk{graph};

    while(state.KeepRunning()) {
        state.PauseTiming();
        common::NodeID s{distr(gen)};
        state.ResumeTiming();

        benchmark::DoNotOptimize(dijk.distancesFrom(s));
    }
}
