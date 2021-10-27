//all the includes you want to use before the gtest include

#include "../../globals.hpp"
#include <fmt/ranges.h>
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(OffsetArrayTest, OffsetArrayNumberOfNodesTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.numberOfNodes(), 5);
}

TEST(OffsetArrayTest, CHOffsetArrayNumberOfNodesTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.numberOfNodes(), 5);
}

TEST(OffsetArrayTest, OffsetArrayNumberOfEdgesTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.numberOfEdges(), 9);
}

TEST(OffsetArrayTest, CHOffsetArrayNumberOfEdgesTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.numberOfEdges(), 10);
}

TEST(OffsetArrayTest, OffsetArrayConnectionTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{4}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{0}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{4}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{3}));

    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{4}));
}

TEST(OffsetArrayTest, CHOffsetArrayConnectionTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{4}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{0}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{4}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{1}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{3}));

    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{0}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{1}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{2}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{3}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{4}, common::NodeID{4}));
}

TEST(OffsetArrayTest, OffsetArrayBackConnectionTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{1}, common::NodeID{0}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{2}, common::NodeID{0}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{4}, common::NodeID{0}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{0}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{1}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{4}, common::NodeID{2}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{2}, common::NodeID{3}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{1}, common::NodeID{4}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{3}, common::NodeID{4}));

    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{0}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{3}, common::NodeID{0}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{0}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{1}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{2}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{3}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{4}, common::NodeID{1}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{2}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{3}, common::NodeID{2}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{0}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{1}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{3}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{4}, common::NodeID{3}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{0}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{2}, common::NodeID{4}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBackwardBetween(common::NodeID{4}, common::NodeID{4}));
}

TEST(OffsetArrayTest, OffsetArrayNodeExistanceTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.nodeExists(common::NodeID{0}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{1}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{2}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{3}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{4}));

    EXPECT_FALSE(graph.nodeExists(common::NodeID{5}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{6}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{7}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{static_cast<std::size_t>(-1)}));
}

TEST(OffsetArrayTest, CHOffsetArrayNodeExistanceTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.nodeExists(common::NodeID{0}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{1}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{2}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{3}));
    EXPECT_TRUE(graph.nodeExists(common::NodeID{4}));

    EXPECT_FALSE(graph.nodeExists(common::NodeID{5}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{6}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{7}));
    EXPECT_FALSE(graph.nodeExists(common::NodeID{static_cast<std::size_t>(-1)}));
}

TEST(OffsetArrayTest, OffsetArrayEdgeExistanceTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.edgeExists(common::EdgeID{0}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{1}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{2}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{3}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{4}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{5}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{6}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{7}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{8}));

    EXPECT_FALSE(graph.edgeExists(common::EdgeID{9}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{10}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{11}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{12}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{static_cast<std::size_t>(-1)}));
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeExistanceTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_TRUE(graph.edgeExists(common::EdgeID{0}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{1}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{2}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{3}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{4}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{5}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{6}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{7}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{8}));
    EXPECT_TRUE(graph.edgeExists(common::EdgeID{9}));

    EXPECT_FALSE(graph.edgeExists(common::EdgeID{10}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{11}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{12}));
    EXPECT_FALSE(graph.edgeExists(common::EdgeID{static_cast<std::size_t>(-1)}));
}

TEST(OffsetArrayTest, OffsetArrayEdgeWeightTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    auto id = graph.getForwardEdgeIDBetween(common::NodeID{0}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{9});

    id = graph.getForwardEdgeIDBetween(common::NodeID{0}, common::NodeID{4}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{7});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{0}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{6});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{5});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{4}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{3}, common::NodeID{2}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{3});

    id = graph.getForwardEdgeIDBetween(common::NodeID{4}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{2});

    id = graph.getForwardEdgeIDBetween(common::NodeID{4}, common::NodeID{3}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{1});
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeWeightTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    auto id = graph.getForwardEdgeIDBetween(common::NodeID{0}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{9});

    id = graph.getForwardEdgeIDBetween(common::NodeID{0}, common::NodeID{2}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{8});

    id = graph.getForwardEdgeIDBetween(common::NodeID{0}, common::NodeID{4}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{7});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{0}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{6});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{5});

    id = graph.getForwardEdgeIDBetween(common::NodeID{2}, common::NodeID{4}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{3}, common::NodeID{2}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{3});

    id = graph.getForwardEdgeIDBetween(common::NodeID{4}, common::NodeID{1}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{2});

    id = graph.getForwardEdgeIDBetween(common::NodeID{4}, common::NodeID{2}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{4}, common::NodeID{3}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{1});
}

TEST(OffsetArrayTest, OffsetArrayEdgeSpeedTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{4});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{0});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{4});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{3}, common::NodeID{2});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{3});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeSpeedTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{4});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{0});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{4});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{3}, common::NodeID{2});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{1});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{2});
    EXPECT_EQ(edge->getSpeed(), common::Speed{-1});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{3});
    EXPECT_EQ(edge->getSpeed(), common::Speed{50});
}

TEST(OffsetArrayTest, OffsetArrayEdgeTypeTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{4});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{0});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{4});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{3}, common::NodeID{2});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{3});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeTypeTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{0}, common::NodeID{4});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{0});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{2}, common::NodeID{4});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{3}, common::NodeID{2});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{1});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{2});
    EXPECT_EQ(edge->getEdgeType(), common::Type{0});

    edge = graph.getForwardEdgeBetween(common::NodeID{4}, common::NodeID{3});
    EXPECT_EQ(edge->getEdgeType(), common::Type{3});
}

TEST(OffsetArrayTest, OffsetArrayNodeID2Test)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getID2(), common::NodeID{100});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getID2(), common::NodeID{101});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getID2(), common::NodeID{102});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getID2(), common::NodeID{103});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getID2(), common::NodeID{104});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeID2Test)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getID2(), common::NodeID{100});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getID2(), common::NodeID{101});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getID2(), common::NodeID{102});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getID2(), common::NodeID{103});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getID2(), common::NodeID{104});
}

TEST(OffsetArrayTest, OffsetArrayNodeLatTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getLat(), common::Latitude{49.00});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getLat(), common::Latitude{49.01});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getLat(), common::Latitude{49.02});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getLat(), common::Latitude{49.03});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getLat(), common::Latitude{49.04});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeLatTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getLat(), common::Latitude{49.00});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getLat(), common::Latitude{49.01});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getLat(), common::Latitude{49.02});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getLat(), common::Latitude{49.03});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getLat(), common::Latitude{49.04});
}

TEST(OffsetArrayTest, OffsetArrayNodeLngTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getLng(), common::Longitude{10.00});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getLng(), common::Longitude{10.01});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getLng(), common::Longitude{10.02});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getLng(), common::Longitude{10.03});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getLng(), common::Longitude{10.04});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeLngTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getLng(), common::Longitude{10.00});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getLng(), common::Longitude{10.01});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getLng(), common::Longitude{10.02});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getLng(), common::Longitude{10.03});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getLng(), common::Longitude{10.04});
}

TEST(OffsetArrayTest, OffsetArrayNodeElevationTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeElevationTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    const auto *node = graph.getNode(common::NodeID{0});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{1});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{2});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{3});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});

    node = graph.getNode(common::NodeID{4});
    EXPECT_EQ(node->getElevation(), common::Elevation{0});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeLevelTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.getNodeLevel(common::NodeID{0}), common::NodeLevel{3});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{1}), common::NodeLevel{0});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{2}), common::NodeLevel{2});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{3}), common::NodeLevel{0});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{4}), common::NodeLevel{1});
}

TEST(OffsetArrayTest, CHOffsetArrayNestedEdgeTest)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    auto edge = graph.getEdge(common::EdgeID{8});

    ASSERT_TRUE(edge->isShortcut());
    EXPECT_EQ(edge->getShortcutUnsafe().first, common::EdgeID{9});
    EXPECT_EQ(edge->getShortcutUnsafe().second, common::EdgeID{6});
    EXPECT_EQ(edge->getShortcut().value().first, common::EdgeID{9});
    EXPECT_EQ(edge->getShortcut().value().second, common::EdgeID{6});

    edge = graph.getEdge(common::EdgeID{0});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{1});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{2});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{3});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{4});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{5});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{6});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{7});
    EXPECT_FALSE(edge->isShortcut());

    edge = graph.getEdge(common::EdgeID{9});
    EXPECT_FALSE(edge->isShortcut());
}

TEST(OffsetArrayTest, OffsetArrayNodeSortingTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());
    auto [perm, inv_perm] = graph.sortNodesAccordingTo([](const auto &g) {
        return [](auto lhs, auto rhs) {
            return lhs > rhs;
        };
    });

    EXPECT_EQ(perm[0], 4);
    EXPECT_EQ(perm[1], 3);
    EXPECT_EQ(perm[2], 2);
    EXPECT_EQ(perm[3], 1);
    EXPECT_EQ(perm[4], 0);

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getForwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            EXPECT_EQ(src.get(), i);
        }
    }

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getBackwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            EXPECT_EQ(src.get(), i);
        }
    }

    auto id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{9});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{7});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{6});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{5});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{3});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{2});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{1});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeSortingTest1)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    auto [perm, inv_perm] = graph.sortNodesAccordingTo([](const auto &g) {
        return [](auto lhs, auto rhs) {
            return lhs > rhs;
        };
    });

    EXPECT_EQ(perm[0], 4);
    EXPECT_EQ(perm[1], 3);
    EXPECT_EQ(perm[2], 2);
    EXPECT_EQ(perm[3], 1);
    EXPECT_EQ(perm[4], 0);

    EXPECT_EQ(graph.getNodeLevel(common::NodeID{inv_perm[0]}), common::NodeLevel{3});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{inv_perm[1]}), common::NodeLevel{0});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{inv_perm[2]}), common::NodeLevel{2});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{inv_perm[3]}), common::NodeLevel{0});
    EXPECT_EQ(graph.getNodeLevel(common::NodeID{inv_perm[4]}), common::NodeLevel{1});

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getForwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_NE(src_lvl.get(), trg_lvl.get());
        }
    }

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getBackwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_NE(src_lvl.get(), trg_lvl.get());
        }
    }

    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}));

    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[1]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[2]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[4]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[2]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[1]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[4]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[4]}));


    auto id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{9});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{8});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{7});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{6});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{5});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{3});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{2});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{1});
}

TEST(OffsetArrayTest, CHOffsetArrayNodeSortingTest2)
{
    auto example_graph = data_dir + "ch-fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<true>, graphs::FMIEdge<true>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    auto [perm, inv_perm] = graph.sortNodesAccordingTo([](const auto &g) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_lvl = g.getNodeLevelUnsafe(lhs);
            const auto rhs_lvl = g.getNodeLevelUnsafe(rhs);
            return lhs_lvl > rhs_lvl;
        };
    });

    EXPECT_EQ(perm[0], 0);
    EXPECT_EQ(perm[1], 2);
    EXPECT_EQ(perm[2], 4);
    EXPECT_EQ(perm[3], 1);
    EXPECT_EQ(perm[4], 3);

    EXPECT_EQ(graph.getForwardEdgeIDsOf(common::NodeID{inv_perm[0]}).size(), 3);
    EXPECT_EQ(graph.getForwardEdgeIDsOf(common::NodeID{inv_perm[1]}).size(), 0);
    EXPECT_EQ(graph.getForwardEdgeIDsOf(common::NodeID{inv_perm[2]}).size(), 3);
    EXPECT_EQ(graph.getForwardEdgeIDsOf(common::NodeID{inv_perm[3]}).size(), 1);
    EXPECT_EQ(graph.getForwardEdgeIDsOf(common::NodeID{inv_perm[4]}).size(), 3);

    EXPECT_EQ(graph.getBackwardEdgeIDsOf(common::NodeID{inv_perm[0]}).size(), 1);
    EXPECT_EQ(graph.getBackwardEdgeIDsOf(common::NodeID{inv_perm[1]}).size(), 3);
    EXPECT_EQ(graph.getBackwardEdgeIDsOf(common::NodeID{inv_perm[2]}).size(), 3);
    EXPECT_EQ(graph.getBackwardEdgeIDsOf(common::NodeID{inv_perm[3]}).size(), 1);
    EXPECT_EQ(graph.getBackwardEdgeIDsOf(common::NodeID{inv_perm[4]}).size(), 2);

    EXPECT_EQ(graph.getNodeLevelUnsafe(common::NodeID{0}), common::NodeLevel{3});
    EXPECT_EQ(graph.getNodeLevelUnsafe(common::NodeID{1}), common::NodeLevel{2});
    EXPECT_EQ(graph.getNodeLevelUnsafe(common::NodeID{2}), common::NodeLevel{1});
    EXPECT_EQ(graph.getNodeLevelUnsafe(common::NodeID{3}), common::NodeLevel{0});
    EXPECT_EQ(graph.getNodeLevelUnsafe(common::NodeID{4}), common::NodeLevel{0});

    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}));
    EXPECT_TRUE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}));

    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[1]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[2]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[4]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[2]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[1]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[3]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[4]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[0]}));
    EXPECT_FALSE(graph.checkIfEdgeExistsBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[4]}));

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getForwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_NE(src_lvl.get(), trg_lvl.get());
        }
    }

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getBackwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_NE(src_lvl.get(), trg_lvl.get());
        }
    }


    auto id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{9});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{8});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{7});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{6});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{5});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{3});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{2});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{4});

    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}).value();
    EXPECT_EQ(graph.getEdgeWeight(id).value(), common::Weight{1});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[0]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[1]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[1]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[0]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[0]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[2]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[0]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[0]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[4]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[4]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[0]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[0]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[0]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[0]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[0]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[2]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[1]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[1]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[2]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[4]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[4]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[2]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[3]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[2]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[3]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[3]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[1]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[4]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[1]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[1]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[1]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[4]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[2]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[4]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[2]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[2]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[2]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[4]});


    id = graph.getForwardEdgeIDBetween(common::NodeID{inv_perm[4]}, common::NodeID{inv_perm[3]}).value();
    EXPECT_EQ(graph.getEdge(id)->getSrc(), common::NodeID{inv_perm[4]});
    EXPECT_EQ(graph.getEdge(id)->getTrg(), common::NodeID{inv_perm[3]});

    id = graph.getBackwardEdgeIDBetween(common::NodeID{inv_perm[3]}, common::NodeID{inv_perm[4]}).value();
    EXPECT_EQ(graph.getBackwardEdge(id)->getSrc(), common::NodeID{inv_perm[3]});
    EXPECT_EQ(graph.getBackwardEdge(id)->getTrg(), common::NodeID{inv_perm[4]});
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeIdDeleteTest1)
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

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getForwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_LT(src_lvl, trg_lvl);
        }
    }

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getBackwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_LT(src_lvl, trg_lvl);
        }
    }
}

TEST(OffsetArrayTest, CHOffsetArrayEdgeIdDeleteAndSortTest1)
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

    auto perm = graph.sortNodesAccordingTo([](const auto &graph) {
        return [&](const auto lhs, const auto rhs) {
            const auto lhs_lvl = graph.getNodeLevelUnsafe(lhs);
            const auto rhs_lvl = graph.getNodeLevelUnsafe(rhs);
            return lhs_lvl > rhs_lvl;
        };
    });

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getForwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto *edge = graph.getEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_LT(src_lvl, trg_lvl);
        }
    }

    for(std::size_t i = 0; i < graph.numberOfNodes(); i++) {
        auto ids = graph.getBackwardEdgeIDsOf(common::NodeID{i});
        for(auto id : ids) {
            const auto edge = graph.getBackwardEdge(id);
            const auto src = edge->getSrc();
            const auto trg = edge->getTrg();
            const auto src_lvl = graph.getNodeLevelUnsafe(src);
            const auto trg_lvl = graph.getNodeLevelUnsafe(trg);

            EXPECT_EQ(src.get(), i);
            EXPECT_LT(src_lvl, trg_lvl);
        }
    }
}
