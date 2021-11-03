#pragma once
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>

void SimpleToyGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/fmi-example.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph));
    }
}

void CHToyGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/ch-fmi-example.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph));
    }
}

void SimpleStgGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/stgtregbz.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph));
    }
}

void CHStgGraphParsing(benchmark::State& state)
{
    const auto* const example_graph = "../data/ch-stgtregbz.txt";
    for(auto _ : state) {
        benchmark::DoNotOptimize(parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph));
    }
}
