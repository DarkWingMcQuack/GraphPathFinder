//all the includes you want to use before the gtest include

#include "../../globals.hpp"
#include <graphs/edges/FMIEdge.hpp>
#include <graphs/nodes/FMINode.hpp>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <parsing/offsetarray/Parser.hpp>
#include <string_view>

#include <gtest/gtest.h>


TEST(OffsetArrayTest, OffsetArrayParsingTest)
{
    auto example_graph = data_dir + "fmi-example.txt";
    auto graph_opt = parsing::parseFromFMIFile<graphs::FMINode<false>, graphs::FMIEdge<false>>(example_graph);

    ASSERT_TRUE(graph_opt);
}
