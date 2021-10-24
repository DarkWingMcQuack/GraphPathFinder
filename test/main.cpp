#include "globals.hpp"
#include <gtest/gtest.h>

auto main(int argc, char **argv)
    -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    assert(argc == 2); // gtest leaves unparsed arguments for you
    data_dir = argv[1];

    return RUN_ALL_TESTS();
}
