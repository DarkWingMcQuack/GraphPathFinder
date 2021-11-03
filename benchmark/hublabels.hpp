#pragma once

#include <algorithms/distoracle/hublabels/HubLabelCalculator.hpp>
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>

inline auto HubLabelsGraphPreparation(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    for(auto _ : state) {
        state.PauseTiming();
        auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
        state.ResumeTiming();
        benchmark::DoNotOptimize(algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph)));
    }
}

inline auto HubLabelsComputation(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    auto graph = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    graph = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph));
    for(auto _ : state) {
        algorithms::distoracle::HubLabelCalculator calculator{graph};
        benchmark::DoNotOptimize(calculator.constructHubLabelLookupInParallel());
    }
}

inline auto HubLabelsOneToOne(benchmark::State& state)
    -> void
{
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    // needs to be static otherwise it gets recalculated every iteration
    // fixture is not possible because it segfaults(and it is not my problem)
    static auto graph = [&] {
        auto g = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
        return algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(g));
    }();

    static algorithms::distoracle::HubLabelCalculator calculator{graph};
    static const auto lookup = calculator.constructHubLabelLookupInParallel();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distr(0, lookup.numberOfNodes() - 1);

    while(state.KeepRunning()) {
        state.PauseTiming();
        common::NodeID s{distr(gen)};
        common::NodeID t{distr(gen)};
        state.ResumeTiming();

        benchmark::DoNotOptimize(lookup.distanceBetween(s, t));
    }
}
