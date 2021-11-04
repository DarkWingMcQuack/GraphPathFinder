#pragma once
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>

inline auto FMINodeWithoutLevelParsing(benchmark::State& state)
{
    const auto* node_str = "78 238389 48.55053880000000532 9.65097629999999995 0";
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(graphs::FMINode<false>::parse(node_str));
    }
}

inline auto FMINodeWithLevelParsing(benchmark::State& state)
{
    const auto* node_str = "70 238381 48.5433115 9.6478376 0 3";
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(graphs::FMINode<true>::parse(node_str));
    }
}

inline auto FMIEdgeWithoutShortcutParsing(benchmark::State& state)
{
    const auto* edge_str = "1132108 1128845 92 12 45";
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(graphs::FMIEdge<false>::parse(edge_str));
    }
}

inline auto FMIEdgeWithShortcutParsing(benchmark::State& state)
{
    const auto* edge_str = "1132108 1128846 277 0 -1 4148254 4138520";
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(graphs::FMIEdge<false>::parse(edge_str));
    }
}

inline void SimpleToyGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/fmi-example.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph));
    }
}

inline void CHToyGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/ch-fmi-example.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph));
    }
}

inline void SimpleStgGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/stgtregbz.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph));
    }
}

inline void CHStgGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/ch-stgtregbz.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph));
    }
}
