//all the includes you want to use before the gtest include
#include "../../globals.hpp"
#include <algorithms/distoracle/PHAST.hpp>
#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(PHASTTest, ToyAllToAllPHASTTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForPHAST(std::move(graph));

    algorithms::distoracle::PHAST phast{graph};
    algorithms::distoracle::CHDijkstra dijkstra{graph};

    auto dists = phast.distancesFrom(common::NodeID{0});

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        EXPECT_EQ(dists[i], dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{i}));
    }

    dists = phast.distancesFrom(common::NodeID{1});
    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        EXPECT_EQ(dists[i], dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{i}));
    }

    dists = phast.distancesFrom(common::NodeID{2});
    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        EXPECT_EQ(dists[i], dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{i}));
    }

    dists = phast.distancesFrom(common::NodeID{3});
    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        EXPECT_EQ(dists[i], dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{i}));
    }

    dists = phast.distancesFrom(common::NodeID{4});
    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        EXPECT_EQ(dists[i], dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{i}));
    }
}
