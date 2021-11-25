#include "utils/Timer.hpp"

#include <CLI/CLI.hpp>
#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <algorithms/distoracle/hublabels/HubLabelCalculator.hpp>
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <algorithms/distoracle/patches/PatchGrower.hpp>
#include <algorithms/pathfinding/dijkstra/Dijkstra.hpp>
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

auto parseCLI(int argc, char* argv[]) -> std::pair<std::string, std::string>
{
    CLI::App app{"Example calculation patches using hublabels"};

    std::string graph_file;
    std::string ch_graph_file;

    app.add_option("-g,--graph",
                   graph_file,
                   "graph file in FMI-format")
        ->required();

    app.add_option("-c,--chgraph",
                   ch_graph_file,
                   "ch graph file in FMI-format")
        ->required();

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        auto help = app.help();
        fmt::print("{}\n", help);

        std::exit(-1);
    }

    return std::pair{std::move(graph_file),
                     std::move(ch_graph_file)};
}

auto getNumber(std::size_t max) -> common::NodeID
{
    std::size_t n;
    std::cin >> n; // input the length

    if(n > max) {
        fmt::print("number to large, needs to be <={}\n", max);
        return getNumber(max);
    }

    return common::NodeID{n};
}

auto getMiddleNode(const graphs::Path& path) noexcept
    -> common::NodeID
{
    const auto idx = path.getNumberOfNodes() / 2;
    return path[static_cast<int>(idx)];
}


auto main(int argc, char* argv[])
    -> int
{
    const auto [graph_file, ch_graph_file] = parseCLI(argc, argv);

    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(graph_file);
    auto ch_graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(ch_graph_file);

    if(!graph_opt) {
        fmt::print("unable to parse graph: {}\n", graph_file);
        std::exit(-1);
    }

    if(!ch_graph_opt) {
        fmt::print("unable to parse graph: {}\n", ch_graph_file);
        std::exit(-1);
    }

    const auto ch_graph = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(ch_graph_opt.value()));
    const auto graph = std::move(graph_opt.value());

    algorithms::distoracle::HubLabelCalculator calculator{ch_graph};
    auto hl_lookup = calculator.constructHubLabelLookupInParallel();

    algorithms::pathfinding::Dijkstra path_dijkstra{graph};
    algorithms::distoracle::PatchGrower patch_grower{graph, hl_lookup};

    while(true) {
        fmt::print("source node:");
        const auto src = getNumber(graph.numberOfNodes() - 1);
        fmt::print("target node:");
        const auto trg = getNumber(graph.numberOfNodes() - 1);

        const auto path_opt = path_dijkstra.pathBetween(src, trg);
        if(!path_opt) {
            fmt::print("unable to find path between {} and {}\n", src.get(), trg.get());
            continue;
        }

        const auto& path = path_opt.value();
        const auto barrier = getMiddleNode(path);

        if(barrier == src) {
            fmt::print("barrier was equal to src\n");
            continue;
        }

        if(barrier == trg) {
            fmt::print("barrier was equal to src\n");
            continue;
        }

        fmt::print("barrier {}\n", barrier.get());

        const auto patch = patch_grower.grow(std::vector{src},
                                             barrier,
                                             std::vector{trg});

        const auto weight = patch.weight();

        fmt::print("weight of calculated weight: {}\n", weight);
    }
}
