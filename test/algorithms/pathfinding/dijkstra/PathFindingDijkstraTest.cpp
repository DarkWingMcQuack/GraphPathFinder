//all the includes you want to use before the gtest include

#include "../../../globals.hpp"
#include <algorithms/pathfinding/dijkstra/PathFindingDijkstra.hpp>
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(PathFindingDijkstraTest, MixedSourcesTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    algorithms::pathfinding::PathFindingDijkstra dijkstra{graph};

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

TEST(PathFindingDijkstraTest, SameSourcesTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    algorithms::pathfinding::PathFindingDijkstra dijkstra{graph};

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


TEST(PathFindingDijkstraTest, SameSourcesPathTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    algorithms::pathfinding::PathFindingDijkstra dijkstra{graph};

    auto actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    auto actual = std::move(actual_opt.value());
    auto expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{1}}, common::Weight{9}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{2}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{4}, common::NodeID{3}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{4}}, common::Weight{7}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{0});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{2});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{3});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{4});
    ASSERT_FALSE(actual_opt);


    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{0}}, common::Weight{6}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{1}}, common::Weight{5}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{4}, common::NodeID{3}}, common::Weight{5}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{4}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);


    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{0}}, common::Weight{9}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{1}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{4}}, common::Weight{7}};
    EXPECT_EQ(actual, expected);


    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}, common::NodeID{2}, common::NodeID{0}}, common::Weight{10}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{1}}, common::Weight{2}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}, common::NodeID{2}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}}, common::Weight{1}};
    EXPECT_EQ(actual, expected);
}

TEST(PathFindingDijkstraTest, MixedSourcesPathTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    algorithms::pathfinding::PathFindingDijkstra dijkstra{graph};

    auto actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    auto actual = std::move(actual_opt.value());
    auto expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{1}}, common::Weight{9}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{3});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{2}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{0});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{4}, common::NodeID{3}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{2});
    ASSERT_FALSE(actual_opt);

    actual_opt = dijkstra.pathBetween(common::NodeID{0}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{0}, common::NodeID{4}}, common::Weight{7}};
    EXPECT_EQ(actual, expected);



    actual_opt = dijkstra.pathBetween(common::NodeID{1}, common::NodeID{4});
    ASSERT_FALSE(actual_opt);



    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{0}}, common::Weight{6}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{0}}, common::Weight{9}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{1}}, common::Weight{5}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{1}}, common::Weight{8}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{4}, common::NodeID{3}}, common::Weight{5}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}, common::NodeID{2}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{2});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{1});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{1}}, common::Weight{2}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{2}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{2}, common::NodeID{4}}, common::Weight{4}};
    EXPECT_EQ(actual, expected);


    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{0});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}, common::NodeID{2}, common::NodeID{0}}, common::Weight{10}};
    EXPECT_EQ(actual, expected);


    actual_opt = dijkstra.pathBetween(common::NodeID{3}, common::NodeID{4});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{3}, common::NodeID{2}, common::NodeID{4}}, common::Weight{7}};
    EXPECT_EQ(actual, expected);

    actual_opt = dijkstra.pathBetween(common::NodeID{4}, common::NodeID{3});
    ASSERT_TRUE(actual_opt);
    actual = std::move(actual_opt.value());
    expected = graphs::Path{std::vector{common::NodeID{4}, common::NodeID{3}}, common::Weight{1}};
    EXPECT_EQ(actual, expected);
}
