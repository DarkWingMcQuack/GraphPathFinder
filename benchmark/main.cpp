#include <benchmark/benchmark.h>

#include "parsing.hpp"
#include "dijkstra.hpp"
#include "chdijkstra.hpp"
#include "phast.hpp"

//parsing
BENCHMARK(SimpleToyGraphParsing)->Unit(benchmark::kMillisecond);
BENCHMARK(CHToyGraphParsing)->Unit(benchmark::kMillisecond);
BENCHMARK(SimpleStgGraphParsing)->Unit(benchmark::kSecond)->Iterations(2);
BENCHMARK(CHStgGraphParsing)->Unit(benchmark::kSecond)->Iterations(2);

//dijkstra
BENCHMARK_REGISTER_F(DijkstraFixture, DijkstraOneToOneDistanceQueryBenchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(DijkstraFixture, DijkstraInitializationBenchmark)->Unit(benchmark::kMillisecond);

//chdijkstra
BENCHMARK_REGISTER_F(CHDijkstraFixture, CHDijkstraOneToOneDistanceQueryBenchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(CHDijkstraFixture, CHDijkstraInitializationBenchmark)->Unit(benchmark::kMillisecond);

//phast
BENCHMARK_REGISTER_F(PHASTFixture, PHASTOneToAllDistanceQueryBenchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(PHASTFixture, PHASTInitializationBenchmark)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
