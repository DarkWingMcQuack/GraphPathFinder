// all the includes you want to use before the gtest include
#include <fmt/ranges.h>
#include <utils/Permutation.hpp>

#include <gtest/gtest.h>


TEST(PermutationTest, ApplyPermutationTest1)
{
    std::vector<std::size_t> permutation{5, 4, 3, 2, 1, 0};
    std::vector data{0, 1, 2, 3, 4, 5};

    data = util::applyPermutation(std::move(data),
                                  std::move(permutation));

    EXPECT_EQ(data[0], 5);
    EXPECT_EQ(data[1], 4);
    EXPECT_EQ(data[2], 3);
    EXPECT_EQ(data[3], 2);
    EXPECT_EQ(data[4], 1);
    EXPECT_EQ(data[5], 0);
}

TEST(PermutationTest, ApplyPermutationTest2)
{
    std::vector<std::size_t> permutation{5, 4, 0, 1, 2, 3};
    std::vector data{0, 1, 2, 3, 4, 5};

    data = util::applyPermutation(std::move(data), permutation);

    EXPECT_EQ(data[0], 5);
    EXPECT_EQ(data[1], 4);
    EXPECT_EQ(data[2], 0);
    EXPECT_EQ(data[3], 1);
    EXPECT_EQ(data[4], 2);
    EXPECT_EQ(data[5], 3);
}

TEST(PermutationTest, ApplyPermutationTest3)
{
    std::vector<std::size_t> permutation{5, 4, 0, 1, 2, 3, 6, 9, 8, 7, 10};
    std::vector data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    data = util::applyPermutation(std::move(data), permutation);

    EXPECT_EQ(data[0], 5);
    EXPECT_EQ(data[1], 4);
    EXPECT_EQ(data[2], 0);
    EXPECT_EQ(data[3], 1);
    EXPECT_EQ(data[4], 2);
    EXPECT_EQ(data[5], 3);
    EXPECT_EQ(data[6], 6);
    EXPECT_EQ(data[7], 9);
    EXPECT_EQ(data[8], 8);
    EXPECT_EQ(data[9], 7);
    EXPECT_EQ(data[10], 10);
}

TEST(PermutationTest, ApplyPermutationTest4)
{
    std::vector<std::size_t> permutation{5, 4, 0, 1, 2, 3, 6, 9, 8, 7, 10};
    std::vector data{std::vector{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}};

    data = util::applyPermutation(std::move(data), permutation);

    EXPECT_EQ(data[0], (std::vector{5, 6}));
    EXPECT_EQ(data[1], (std::vector{4, 5}));
    EXPECT_EQ(data[2], (std::vector{0, 1}));
    EXPECT_EQ(data[3], (std::vector{1, 2}));
    EXPECT_EQ(data[4], (std::vector{2, 3}));
    EXPECT_EQ(data[5], (std::vector{3, 4}));
    EXPECT_EQ(data[6], (std::vector{6, 7}));
    EXPECT_EQ(data[7], (std::vector{9, 10}));
    EXPECT_EQ(data[8], (std::vector{8, 9}));
    EXPECT_EQ(data[9], (std::vector{7, 8}));
    EXPECT_EQ(data[10], (std::vector{10, 11}));
}

TEST(PermutationTest, InversePermutationTest1)
{
    std::vector<std::size_t> permutation{5, 4, 3, 2, 1, 0};
    auto inverse = util::inversePermutation(permutation);

    EXPECT_EQ(permutation[inverse[0]], 0);
    EXPECT_EQ(permutation[inverse[1]], 1);
    EXPECT_EQ(permutation[inverse[2]], 2);
    EXPECT_EQ(permutation[inverse[3]], 3);
    EXPECT_EQ(permutation[inverse[4]], 4);
    EXPECT_EQ(permutation[inverse[5]], 5);

    EXPECT_EQ(inverse[permutation[0]], 0);
    EXPECT_EQ(inverse[permutation[1]], 1);
    EXPECT_EQ(inverse[permutation[2]], 2);
    EXPECT_EQ(inverse[permutation[3]], 3);
    EXPECT_EQ(inverse[permutation[4]], 4);
    EXPECT_EQ(inverse[permutation[5]], 5);
}

TEST(PermutationTest, InversePermutationTest2)
{
    std::vector<std::size_t> permutation{5, 3, 2, 4, 0, 1};
    auto inverse = util::inversePermutation(permutation);

    EXPECT_EQ(permutation[inverse[0]], 0);
    EXPECT_EQ(permutation[inverse[1]], 1);
    EXPECT_EQ(permutation[inverse[2]], 2);
    EXPECT_EQ(permutation[inverse[3]], 3);
    EXPECT_EQ(permutation[inverse[4]], 4);
    EXPECT_EQ(permutation[inverse[5]], 5);

    EXPECT_EQ(inverse[permutation[0]], 0);
    EXPECT_EQ(inverse[permutation[1]], 1);
    EXPECT_EQ(inverse[permutation[2]], 2);
    EXPECT_EQ(inverse[permutation[3]], 3);
    EXPECT_EQ(inverse[permutation[4]], 4);
    EXPECT_EQ(inverse[permutation[5]], 5);
}

TEST(PermutationTest, InversePermutationTest3)
{
    std::vector<std::size_t> permutation{5, 3, 2, 4, 0, 1, 6, 10, 9, 8, 7};
    auto inverse = util::inversePermutation(permutation);

    EXPECT_EQ(permutation[inverse[0]], 0);
    EXPECT_EQ(permutation[inverse[1]], 1);
    EXPECT_EQ(permutation[inverse[2]], 2);
    EXPECT_EQ(permutation[inverse[3]], 3);
    EXPECT_EQ(permutation[inverse[4]], 4);
    EXPECT_EQ(permutation[inverse[5]], 5);
    EXPECT_EQ(permutation[inverse[6]], 6);
    EXPECT_EQ(permutation[inverse[7]], 7);
    EXPECT_EQ(permutation[inverse[8]], 8);
    EXPECT_EQ(permutation[inverse[9]], 9);
    EXPECT_EQ(permutation[inverse[10]], 10);

    EXPECT_EQ(inverse[permutation[0]], 0);
    EXPECT_EQ(inverse[permutation[1]], 1);
    EXPECT_EQ(inverse[permutation[2]], 2);
    EXPECT_EQ(inverse[permutation[3]], 3);
    EXPECT_EQ(inverse[permutation[4]], 4);
    EXPECT_EQ(inverse[permutation[5]], 5);
    EXPECT_EQ(inverse[permutation[6]], 6);
    EXPECT_EQ(inverse[permutation[7]], 7);
    EXPECT_EQ(inverse[permutation[8]], 8);
    EXPECT_EQ(inverse[permutation[9]], 9);
    EXPECT_EQ(inverse[permutation[10]], 10);
}


TEST(PermutationTest, CombinePermutationTest1)
{
    std::vector<std::size_t> permutation1{0, 3, 4, 1, 2};
    std::vector<std::size_t> permutation2{0, 2, 4, 1, 3};

    auto combined = util::combine(permutation1, permutation2);

    EXPECT_EQ(combined[0], 0);
    EXPECT_EQ(combined[1], 1);
    EXPECT_EQ(combined[2], 3);
    EXPECT_EQ(combined[3], 2);
    EXPECT_EQ(combined[4], 4);
}

TEST(PermutationTest, CombinePermutationTest2)
{
    std::vector<std::size_t> permutation1{0, 3, 4, 1, 2};
    const auto permutation2 = util::inversePermutation(permutation1);
    const auto combined1 = util::combine(permutation1, permutation2);

    EXPECT_EQ(combined1[0], 0);
    EXPECT_EQ(combined1[1], 1);
    EXPECT_EQ(combined1[2], 2);
    EXPECT_EQ(combined1[3], 3);
    EXPECT_EQ(combined1[4], 4);

    const auto combined2 = util::combine(permutation2, permutation1);

    EXPECT_EQ(combined2[0], 0);
    EXPECT_EQ(combined2[1], 1);
    EXPECT_EQ(combined2[2], 2);
    EXPECT_EQ(combined2[3], 3);
    EXPECT_EQ(combined2[4], 4);
}


