//all the includes you want to use before the gtest include
#include <graphs/nodes/FMINode.hpp>
#include <string_view>

#include <gtest/gtest.h>

TEST(FMINodeTest, FMINodeWithLevelParsingTest)
{
    auto node_opt = graphs::FMINode<true>::parse("61 625272 42.5716490 1.6112217 0 4");

    ASSERT_TRUE(node_opt);
    EXPECT_EQ(node_opt.value().getID2(), common::NodeID{625272});
    EXPECT_EQ(node_opt.value().getLat(), common::Latitude{42.5716490});
    EXPECT_EQ(node_opt.value().getLng(), common::Longitude{1.6112217});
    EXPECT_EQ(node_opt.value().getElevation(), common::Elevation{0});
    EXPECT_EQ(node_opt.value().getLvl(), common::NodeLevel{4});
}

TEST(FMINodeTest, FMINodeWithoutLevelParsingTest)
{
    auto node_opt = graphs::FMINode<false>::parse("61 625272 42.5716490 1.6112217 0");

    ASSERT_TRUE(node_opt);
    EXPECT_EQ(node_opt.value().getID2(), common::NodeID{625272});
    EXPECT_EQ(node_opt.value().getLat(), common::Latitude{42.5716490});
    EXPECT_EQ(node_opt.value().getLng(), common::Longitude{1.6112217});
    EXPECT_EQ(node_opt.value().getElevation(), common::Elevation{0});
}

TEST(FMINodeTest, FailingFMINodeWithoutLevelParsingTest)
{
    auto node_opt = graphs::FMINode<false>::parse("#61 625272 42.5716490 1.6112217 0");
    EXPECT_FALSE(node_opt);

    node_opt = graphs::FMINode<false>::parse("625272 42.5716490 1.6112217 0");
    EXPECT_FALSE(node_opt);

    node_opt = graphs::FMINode<false>::parse("%61 625272 42.5716490 1.6112217 0");
    EXPECT_FALSE(node_opt);
}

TEST(FMINodeTest, FailingFMINodeWithLevelParsingTest)
{
    auto node_opt = graphs::FMINode<true>::parse("%61 625272 42.5716490 1.6112217 0 4");
    EXPECT_FALSE(node_opt);

    node_opt = graphs::FMINode<true>::parse("#61 625272 42.5716490 1.6112217 0 4");
    EXPECT_FALSE(node_opt);

    node_opt = graphs::FMINode<true>::parse("61 625272 42.5716490 1.6112217 0");
    EXPECT_FALSE(node_opt);
}
