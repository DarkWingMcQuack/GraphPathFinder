#include <benchmark/benchmark.h>

#include "parsing.hpp"
#include "dijkstra.hpp"
#include "chdijkstra.hpp"
#include "hublabels.hpp"
#include "phast.hpp"

//parsing
BENCHMARK(SimpleToyGraphParsing)->Unit(benchmark::kMillisecond);
BENCHMARK(CHToyGraphParsing)->Unit(benchmark::kMillisecond);
BENCHMARK(SimpleStgGraphParsing)->Unit(benchmark::kSecond);
BENCHMARK(CHStgGraphParsing)->Unit(benchmark::kSecond);

BENCHMARK(DijkstraInitialization)->Unit(benchmark::kMillisecond);
BENCHMARK(DijkstraOneToOne)->Unit(benchmark::kMillisecond);
BENCHMARK(DijkstraOneToAll)->Unit(benchmark::kMillisecond);

BENCHMARK(CHDijkstraGraphPreparation)->Unit(benchmark::kMillisecond);
BENCHMARK(CHDijkstraInitialization)->Unit(benchmark::kMillisecond);
BENCHMARK(CHDijkstraOneToOne)->Unit(benchmark::kMicrosecond);

BENCHMARK(HubLabelsGraphPreparation)->Unit(benchmark::kMillisecond);
BENCHMARK(HubLabelsComputation)->Unit(benchmark::kSecond);
BENCHMARK(HubLabelsOneToOne)->Unit(benchmark::kMicrosecond);

BENCHMARK(PHASTGraphPreparation)->Unit(benchmark::kMillisecond);
BENCHMARK(PHASTInitialization)->Unit(benchmark::kMillisecond);
BENCHMARK(PHASTOneToAll)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
