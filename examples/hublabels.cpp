#include "utils/Timer.hpp"

#include <CLI/CLI.hpp>
#include <algorithms/distoracle/hublabels/HubLabelCalculator.hpp>
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

auto parseCLI(int argc, char* argv[]) -> std::string
{
    CLI::App app{"Example calculating and using HubLabels to calculate shortest path distances"};

    std::string graph_file;

    app.add_option("-g,--graph",
                   graph_file,
                   "ch-graph file in FMI-format")
        ->required();

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        auto help = app.help();
        fmt::print("{}\n", help);

        std::exit(-1);
    }

    return graph_file;
}

auto getNumber(std::size_t max) -> std::size_t
{
    std::size_t n;
    std::cin >> n; // input the length

    if(n > max) {
        fmt::print("number to large, needs to be <={}", max);
        return getNumber(max);
    }

    return n;
}

auto main(int argc, char* argv[])
    -> int
{
    const auto graph_file = parseCLI(argc, argv);

    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(graph_file);

    if(!graph_opt) {
        fmt::print("unable to parse graph: {}\n", graph_file);
        std::exit(-1);
    }

    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph));

    algorithms::distoracle::HubLabelCalculator calculator{graph};
    auto hl_lookup = calculator.constructHubLabelLookupInParallel();

    while(true) {

        fmt::print("source node:");
        auto src = getNumber(graph.numberOfNodes() - 1);
        fmt::print("target node:");
        auto trg = getNumber(graph.numberOfNodes() - 1);

        utils::Timer t;
        auto dist = hl_lookup.distanceBetween(common::NodeID{src}, common::NodeID{trg});
        auto time = t.elapsed();

        fmt::print("distance: {}\ncalculated in: {}ms", dist.get(), time);
    }
}
