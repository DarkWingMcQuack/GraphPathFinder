//all the includes you want to use before the gtest include
#include <common/Parsing.hpp>
#include <string_view>

#include <gtest/gtest.h>

TEST(ParsingTest, SimpleParsingNodeIDTest)
{
    auto first = common::to<common::NodeID>("10");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::NodeID{10});


    first = common::to<common::NodeID>("10 ");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::NodeID{10});

    first = common::to<common::NodeID>("10 0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::NodeID{10});


    first = common::to<common::NodeID>("10.0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::NodeID{10});

    first = common::to<common::NodeID>("-10");
    ASSERT_FALSE(!!first);
}

TEST(ParsingTest, SimpleParsingWeightTest)
{
    auto first = common::to<common::Weight>("10");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{10});


    first = common::to<common::Weight>("10 ");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{10});

    first = common::to<common::Weight>("10 0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{10});

    first = common::to<common::Weight>("10.0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{10});

    first = common::to<common::Weight>("-10 ");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{-10});

    first = common::to<common::Weight>("-10 0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{-10});


    first = common::to<common::Weight>("-10.0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Weight{-10});
}

TEST(ParsingTest, SimpleParsingLatLngTest)
{
    auto first = common::to<common::Latitude>("10");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{10.0});


    first = common::to<common::Latitude>("10 ");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{10.0});

    first = common::to<common::Latitude>("10 0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{10.0});

    first = common::to<common::Latitude>("10.0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{10.0});

    first = common::to<common::Latitude>("-10 ");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{-10.0});

    first = common::to<common::Latitude>("-10 0");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{-10.0});


    first = common::to<common::Latitude>("-10.5");
    ASSERT_TRUE(!!first);
    EXPECT_EQ(first.value(), common::Latitude{-10.5});

    first = common::to<common::Latitude>("-10.001");
    ASSERT_TRUE(!!first);
    EXPECT_DOUBLE_EQ(first.value().get(), common::Latitude{-10.001}.get());

    first = common::to<common::Latitude>("x10.001");
    ASSERT_TRUE(!first);

    first = common::to<common::Latitude>(".10.001");
    ASSERT_TRUE(!!first);
    EXPECT_DOUBLE_EQ(first.value().get(), common::Latitude{.1}.get());
}
