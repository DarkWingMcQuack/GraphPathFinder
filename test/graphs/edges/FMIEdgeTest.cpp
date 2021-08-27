//all the includes you want to use before the gtest include
#include <graphs/edges/FMIEdge.hpp>
#include <string_view>

#include <gtest/gtest.h>

TEST(FMIEdgeTest, FMIEdgeWithShortcutParsingTest)
{
    auto edge_opt = graphs::FMIEdge<true>::parse("26513 26506 1177 0 -1 94502 63826");
    ASSERT_TRUE(edge_opt);

    auto edge = edge_opt.value();

    EXPECT_EQ(edge.getSrc(), common::NodeID{26513});
    EXPECT_EQ(edge.getTrg(), common::NodeID{26506});
    EXPECT_EQ(edge.getWeight(), common::Weight{1177});
    EXPECT_EQ(edge.getEdgeType(), common::Type{0});
    EXPECT_EQ(edge.getSpeed(), common::Speed{-1});

    auto expected = std::optional{std::pair{common::EdgeID(94502), common::EdgeID(63826)}};
    EXPECT_EQ(edge.getShortcut(), expected);

    auto expected2 = std::pair{common::EdgeID(94502), common::EdgeID(63826)};
    EXPECT_EQ(edge.getShortcutUnsafe(), expected2);
    EXPECT_TRUE(edge.isShortcut());


    edge_opt = graphs::FMIEdge<true>::parse("26513 26504 448 13 5 -1 -1");
    ASSERT_TRUE(edge_opt);

    edge = edge_opt.value();

    EXPECT_EQ(edge.getSrc(), common::NodeID{26513});
    EXPECT_EQ(edge.getTrg(), common::NodeID{26504});
    EXPECT_EQ(edge.getWeight(), common::Weight{448});
    EXPECT_EQ(edge.getEdgeType(), common::Type{13});
    EXPECT_EQ(edge.getSpeed(), common::Speed{5});

    EXPECT_EQ(edge.getShortcut(), std::nullopt);

    EXPECT_FALSE(edge.isShortcut());
}

TEST(FMIEdgeTest, FMIEdgeWithoutShortcutParsingTest)
{
    auto edge_opt = graphs::FMIEdge<false>::parse("26513 26504 448 13 5");
    ASSERT_TRUE(edge_opt);

    auto edge = edge_opt.value();

    EXPECT_EQ(edge.getSrc(), common::NodeID{26513});
    EXPECT_EQ(edge.getTrg(), common::NodeID{26504});
    EXPECT_EQ(edge.getWeight(), common::Weight{448});
    EXPECT_EQ(edge.getEdgeType(), common::Type{13});
    EXPECT_EQ(edge.getSpeed(), common::Speed{5});
}


TEST(FMIEdgeTest, FailingFMIEdgeWithShortcutParsingTest)
{
    auto edge_opt = graphs::FMIEdge<true>::parse("#26513 26506 1177 0 -1 94502 63826");
    ASSERT_FALSE(edge_opt);

    edge_opt = graphs::FMIEdge<true>::parse("%26513 26506 1177 0 -1 94502 63826");
    ASSERT_FALSE(edge_opt);

    edge_opt = graphs::FMIEdge<true>::parse("26506 1177 0 -1 94502 63826");
    ASSERT_FALSE(edge_opt);
}

TEST(FMIEdgeTest, FailingFMIEdgeWithoutShortcutParsingTest)
{
    auto edge_opt = graphs::FMIEdge<true>::parse("#26513 26506 1177 13 5");
    ASSERT_FALSE(edge_opt);

    edge_opt = graphs::FMIEdge<true>::parse("%26513 26506 1177 13 5");
    ASSERT_FALSE(edge_opt);

    edge_opt = graphs::FMIEdge<true>::parse("26506 1177 13 5");
    ASSERT_FALSE(edge_opt);
}
