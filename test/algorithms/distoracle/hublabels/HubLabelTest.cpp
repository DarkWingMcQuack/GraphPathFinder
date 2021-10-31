
// all the includes you want to use before the gtest include

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

TEST(DistanceOracleHubLabelTest, HubLabelNodePermutationTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph.deleteForwardEdgesIDsIf([](const auto &graph) {
        return [&](const auto id) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });

    graph.deleteBackwardEdgesIDsIf([](const auto &graph) {
        return [&](const auto id) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);
            return src_lvl > trg_lvl;
        };
    });

    auto [perm, inv_perm] = graph.sortNodesAccordingTo([](const auto &graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_lvl = graph.getNodeLevelUnsafe(lhs);
            const auto rhs_lvl = graph.getNodeLevelUnsafe(rhs);
            return lhs_lvl > rhs_lvl;
        };
    });

    algorithms::distoracle::HubLabelCalculator calculator{graph};
    auto hl_lookup = calculator.constructHubLabelLookup();

    hl_lookup.applyNodePermutation(std::move(inv_perm), perm);

    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{0}, common::NodeID{1}), common::Weight{9});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{1}, common::NodeID{0}), common::INFINITY_WEIGHT);
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{0}, common::NodeID{2}), common::Weight{8});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{1}, common::NodeID{2}), common::INFINITY_WEIGHT);
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{0}, common::NodeID{3}), common::Weight{8});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{1}, common::NodeID{3}), common::INFINITY_WEIGHT);
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{0}, common::NodeID{4}), common::Weight{7});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{1}, common::NodeID{4}), common::INFINITY_WEIGHT);
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{2}, common::NodeID{0}), common::Weight{6});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{3}, common::NodeID{0}), common::Weight{9});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{4}, common::NodeID{0}), common::Weight{10});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{2}, common::NodeID{1}), common::Weight{5});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{3}, common::NodeID{1}), common::Weight{8});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{4}, common::NodeID{1}), common::Weight{2});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{2}, common::NodeID{3}), common::Weight{5});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{3}, common::NodeID{2}), common::Weight{3});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{4}, common::NodeID{2}), common::Weight{4});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{2}, common::NodeID{4}), common::Weight{4});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{3}, common::NodeID{4}), common::Weight{7});
    EXPECT_EQ(hl_lookup.distanceBetween(common::NodeID{4}, common::NodeID{3}), common::Weight{1});
}
