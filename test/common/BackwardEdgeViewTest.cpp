//all the includes you want to use before the gtest include
#include <common/BackwardEdgeView.hpp>
#include <graphs/edges/SimpleEdge.hpp>
#include <string_view>

#include <gtest/gtest.h>

TEST(BackwardEdgeViewTest, SimpleBackwardEdgeViewTest)
{
    auto edge_opt = graphs::SimpleEdge::parse("10, 20");

    ASSERT_TRUE(!!edge_opt);
    EXPECT_EQ(edge_opt.value().getSrc(), common::NodeID{10});
    EXPECT_EQ(edge_opt.value().getTrg(), common::NodeID{20});

    auto backedge = common::BackwardEdgeView(&edge_opt.value());

    ASSERT_TRUE(!backedge.isNullPtr());
    EXPECT_EQ(backedge.getSrc(), common::NodeID{20});
    EXPECT_EQ(backedge.getTrg(), common::NodeID{10});
}
