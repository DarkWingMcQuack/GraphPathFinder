#pragma once

#include <algorithms/distoracle/hublabels/HubLabelCalculator.hpp>
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <benchmark/benchmark.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <random>


class HubLabelsFixture : public ::benchmark::Fixture
{
public:
    const char* const example_graph = "../data/ch-stgtregbz.txt";
    std::optional<algorithms::distoracle::HubLabelLookup> lookup;
    std::vector<common::NodeID> srcs;
    std::vector<common::NodeID> trgs;
    std::size_t counter = 0;

    HubLabelsFixture()
    {
        auto g = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
        g = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(g));
        algorithms::distoracle::HubLabelCalculator calculator{g};
        lookup = calculator.constructHubLabelLookup();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> distr(0, g.numberOfNodes());

        for(std::size_t i = 0; i < 100; i++) {
            srcs.emplace_back(distr(gen));
            trgs.emplace_back(distr(gen));
        }
    }
};


BENCHMARK_DEFINE_F(HubLabelsFixture, HubLabelsCalculationBenchmark)
(benchmark::State& state)
{
    auto g = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph).value();
    g = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(g));
    algorithms::distoracle::HubLabelCalculator calculator{g};
    for(auto _ : state) {
        benchmark::DoNotOptimize(calculator.constructHubLabelLookup());
    }
};

BENCHMARK_DEFINE_F(HubLabelsFixture, HubLabelsOneToOneDistanceQueryBenchmark)
(benchmark::State& state)
{
    for(auto _ : state) {
        benchmark::DoNotOptimize(lookup->distanceBetween(srcs[counter], trgs[counter]));
        counter = (counter + 1) % 100;
    }
};
