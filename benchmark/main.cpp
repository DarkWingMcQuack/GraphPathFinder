#include <benchmark/benchmark.h>

#include "parsing.hpp"
#include "dijkstra.hpp"
#include "chdijkstra.hpp"
#include "hublabels.hpp"
#include "phast.hpp"

//parsing
BENCHMARK(FMINodeWithoutLevelParsing);
BENCHMARK(FMINodeWithLevelParsing);
BENCHMARK(FMIEdgeWithoutShortcutParsing);
BENCHMARK(FMIEdgeWithShortcutParsing);
BENCHMARK(SimpleToyGraphParsing);
BENCHMARK(CHToyGraphParsing);
BENCHMARK(SimpleStgGraphParsing)->Unit(benchmark::kSecond);
BENCHMARK(CHStgGraphParsing)->Unit(benchmark::kSecond);

BENCHMARK(DijkstraInitialization)->Unit(benchmark::kMicrosecond);
BENCHMARK(DijkstraOneToOne)->Unit(benchmark::kMillisecond)->Iterations(100);
BENCHMARK(DijkstraOneToAll)->Unit(benchmark::kMillisecond)->Iterations(50);

BENCHMARK(CHDijkstraGraphPreparation)->Unit(benchmark::kMicrosecond)->Iterations(10);
BENCHMARK(CHDijkstraInitialization)->Unit(benchmark::kMillisecond);
BENCHMARK(CHDijkstraOneToOne)->Unit(benchmark::kMicrosecond)->Iterations(10000);

BENCHMARK(HubLabelsGraphPreparation)->Unit(benchmark::kMillisecond)->Iterations(10);
BENCHMARK(HubLabelsComputation)->Unit(benchmark::kSecond);
BENCHMARK(HubLabelsOneToOne)->Unit(benchmark::kMicrosecond);

BENCHMARK(PHASTGraphPreparation)->Unit(benchmark::kMillisecond)->Iterations(10);
BENCHMARK(PHASTInitialization)->Unit(benchmark::kMicrosecond);
BENCHMARK(PHASTOneToAll)->Unit(benchmark::kMillisecond)->Iterations(50);

BENCHMARK_MAIN();
