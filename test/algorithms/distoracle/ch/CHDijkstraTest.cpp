//all the includes you want to use before the gtest include

#include "../../../globals.hpp"
#include <algorithms/distoracle/ch/CHDijkstra.hpp>
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(DistanceOracleCHDijkstraTest, MixedSourcesTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    algorithms::distoracle::CHDijkstra dijkstra{graph};


    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{1}), common::Weight{9});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{0}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{2}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{2}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{3}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{3}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{4}), common::Weight{7});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{4}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{0}), common::Weight{6});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{0}), common::Weight{9});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{0}), common::Weight{10});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{1}), common::Weight{5});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{1}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{1}), common::Weight{2});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{3}), common::Weight{5});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{2}), common::Weight{3});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{2}), common::Weight{4});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{4}), common::Weight{4});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{4}), common::Weight{7});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{3}), common::Weight{1});
}

TEST(DistanceOracleCHDijkstraTest, SameSourcesTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    algorithms::distoracle::CHDijkstra dijkstra{graph};

    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{1}), common::Weight{9});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{2}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{3}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{0}, common::NodeID{4}), common::Weight{7});

    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{0}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{2}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{3}), common::INFINITY_WEIGHT);
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{1}, common::NodeID{4}), common::INFINITY_WEIGHT);

    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{0}), common::Weight{6});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{1}), common::Weight{5});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{3}), common::Weight{5});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{2}, common::NodeID{4}), common::Weight{4});

    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{0}), common::Weight{9});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{1}), common::Weight{8});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{2}), common::Weight{3});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{3}, common::NodeID{4}), common::Weight{7});

    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{0}), common::Weight{10});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{1}), common::Weight{2});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{2}), common::Weight{4});
    EXPECT_EQ(dijkstra.distanceBetween(common::NodeID{4}, common::NodeID{3}), common::Weight{1});
}


TEST(DistanceOracleCHDijkstraTest, SourceAndTargetSameTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    algorithms::distoracle::CHDijkstra dijkstra{graph};

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        common::NodeID node{i};

        EXPECT_EQ(dijkstra.distanceBetween(node, node), common::Weight{0});
    }
}


TEST(DistanceOracleCHDijkstraTest, StgRegbzTest)
{
    auto example_graph = data_dir + "ch-stgtregbz.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    graph = algorithms::distoracle::prepareGraphForCHDijkstra(std::move(graph));

    algorithms::distoracle::CHDijkstra dijkstra{graph};


    std::ifstream input_file(data_dir + "stgtregbz-dists.txt",
                             std::ios::in);

    std::string line;
    std::size_t src_s;
    std::size_t trg_s;
    std::int64_t dist_s;
    while(std::getline(input_file, line)) {
        std::stringstream ss{line};
        ss >> src_s >> trg_s >> dist_s;

        common::NodeID src{src_s};
        common::NodeID trg{trg_s};
        common::Weight dist{dist_s};

        const auto dijk_dist = dijkstra.distanceBetween(src, trg);

        EXPECT_EQ(dijk_dist, dist);
    }
}
