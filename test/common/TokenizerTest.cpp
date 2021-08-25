//all the includes you want to use before the gtest include
#include <common/Tokenizer.hpp>
#include <fmt/core.h>
#include <string_view>

#include <gtest/gtest.h>

TEST(TokenizerTest, SimpleTokenizerTest)
{
    using namespace std::literals;
    const auto *data = "first,second,third";

    common::Tokenizer tknz{data, ","};
    auto iter = tknz.begin();

    EXPECT_EQ(*iter, "first"sv);

    iter++;
    EXPECT_EQ(*iter, "second"sv);

    iter++;
    EXPECT_EQ(*iter, "third"sv);

    iter++;
    EXPECT_EQ(iter, tknz.end());
}

TEST(TokenizerTest, SkippingTokenizerTest)
{
    using namespace std::literals;
    const auto *data = "first,,,second,,,,third,,,";

    common::Tokenizer tknz{data, ","};
    auto iter = tknz.begin();

    EXPECT_EQ(*iter, "first"sv);

    iter++;
    EXPECT_EQ(*iter, "second"sv);

    iter++;
    EXPECT_EQ(*iter, "third"sv);

    iter++;
    EXPECT_EQ(iter, tknz.end());
}

TEST(TokenizerTest, FrontSkippingTokenizerTest)
{
    using namespace std::literals;
    const auto *data = ",,,first,,,second,,,,third,,,";

    common::Tokenizer tknz{data, ","};
    auto iter = tknz.begin();

    EXPECT_EQ(*iter, "first"sv);

    iter++;
    EXPECT_EQ(*iter, "second"sv);

    iter++;
    EXPECT_EQ(*iter, "third"sv);

    iter++;
    EXPECT_EQ(iter, tknz.end());
}

TEST(TokenizerTest, ForEachLoopTokenizerTest)
{
    using namespace std::literals;
    const auto *data = ",,,first,,,second,,,,third,,,";

    common::Tokenizer tknz{data, ","};


    for(auto s : tknz) {
        EXPECT_TRUE(s == "first"sv or s == "second"sv or s == "third");
    }
}

TEST(TokenizerTest, TakeFirstNExactTest)
{
    using namespace std::literals;
    const auto *data = ",,,first,,,second,,,,third,,,";

    auto array = common::extractFirstN<3>(data, ",");

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array[0], "first"sv);
    EXPECT_EQ(array[1], "second"sv);
    EXPECT_EQ(array[2], "third"sv);
}

TEST(TokenizerTest, TakeFirstNToViewTest)
{
    using namespace std::literals;
    const auto *data = ",,,first,,,second,,,,third,,,";

    auto array = common::extractFirstN<2>(data, ",");

    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array[0], "first"sv);
    EXPECT_EQ(array[1], "second"sv);
}

TEST(TokenizerTest, TakeFirstNToMuchTest)
{
    using namespace std::literals;
    const auto *data = ",,,first,,,second,,,,third,,,";

    auto array = common::extractFirstN<8>(data, ",");

    EXPECT_EQ(array.size(), 8);
    EXPECT_EQ(array[0], "first"sv);
    EXPECT_EQ(array[1], "second"sv);
    EXPECT_EQ(array[2], "third"sv);
    EXPECT_EQ(array[3], ""sv);
    EXPECT_EQ(array[4], ""sv);
    EXPECT_EQ(array[5], ""sv);
    EXPECT_EQ(array[6], ""sv);
    EXPECT_EQ(array[7], ""sv);
}
