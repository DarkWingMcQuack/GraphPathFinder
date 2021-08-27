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

TEST(OffsetArrayTest, OffsetArrayNumberOfEdgesTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
    auto graph = std::move(graph_opt.value());

    EXPECT_EQ(graph.numberOfEdges(), 9);
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
