
//all the includes you want to use before the gtest include

#include "../../../globals.hpp"
#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <algorithms/distoracle/hublabels/HubLabelCalculator.hpp>
#include <algorithms/distoracle/hublabels/HubLabelLookup.hpp>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(DistanceOracleHubLabelTest, SequentialHubLabelToyTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph));

    algorithms::distoracle::HubLabelCalculator calculator{graph};
    auto hl_lookup = calculator.constructHubLabelLookup();
    algorithms::distoracle::CHDijkstra dijkstra{graph};


    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        for(std::size_t j = 0; j < graph.numberOfNodes(); j++) {
            const auto dij_dist = dijkstra.distanceBetween(common::NodeID{i}, common::NodeID{j});
            const auto hl_dist = hl_lookup.distanceBetween(common::NodeID{i}, common::NodeID{j});

            EXPECT_EQ(hl_dist, dij_dist);
        }
    }
}

TEST(DistanceOracleHubLabelTest, ParallelHubLabelToyTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForHubLabelCalculator(std::move(graph));

    algorithms::distoracle::HubLabelCalculator calculator{graph};
    auto hl_lookup = calculator.constructHubLabelLookupInParallel();
    algorithms::distoracle::CHDijkstra dijkstra{graph};


    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        for(std::size_t j = 0; j < graph.numberOfNodes(); j++) {
            const auto dij_dist = dijkstra.distanceBetween(common::NodeID{i}, common::NodeID{j});
            const auto hl_dist = hl_lookup.distanceBetween(common::NodeID{i}, common::NodeID{j});

            EXPECT_EQ(hl_dist, dij_dist);
        }
    }
}
