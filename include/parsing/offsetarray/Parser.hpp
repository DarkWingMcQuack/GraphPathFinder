
#pragma once

#include <concepts/Parseable.hpp>
#include <fmt/core.h>
#include <fstream>
#include <graphs/offsetarray/OffsetArray.hpp>
#include <utils/MinMax.hpp>

namespace parsing {

template<class Node,
         class Edge,
         bool HasForwardEdges = true,
         bool HasBackwardEdges = true>
requires concepts::Parseable<Node> && concepts::Parseable<Edge>
auto parseFromFMIFile(std::string_view file_path) noexcept
    -> std::optional<graphs::OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>
{
    std::ifstream input_file(file_path.data(),
                             std::ios::in);

    if(!input_file) {
        return std::nullopt;
    }

    std::string line;
    while(line.empty() or line.starts_with('%') or line.starts_with('#')) {
        std::getline(input_file, line);
    }
    std::size_t number_of_nodes = std::stoul(line);

    std::getline(input_file, line);
    while(line.empty() or line.starts_with('%') or line.starts_with('#')) {
        std::getline(input_file, line);
    }
    std::size_t number_of_edges = std::stoul(line);

    std::vector<Node> nodes;
    nodes.reserve(number_of_nodes);

    std::size_t counter = 0;
    while(std::getline(input_file, line) and counter < number_of_nodes) {
        auto node_opt = Node::parse(line);

        if(!node_opt && Edge::parse(line)) {
            break;
        }

        if(!node_opt && !Edge::parse(line)) {
            continue;
        }

        counter++;
        nodes.emplace_back(std::move(node_opt.value()));
    }

    std::vector<Edge> edges;
    edges.reserve(number_of_edges);

    do {
        auto edge_opt = Edge::parse(line);

        if(!edge_opt) {
            continue;
        }

        edges.emplace_back(std::move(edge_opt.value()));
    } while(std::getline(input_file, line));

    return std::optional{
        graphs::OffsetArray<
            Node,
            Edge,
            HasForwardEdges,
            HasBackwardEdges>{
            std::move(nodes),
            std::move(edges)}};
}


template<class Edge,
         bool HasForwardEdges = true,
         bool HasBackwardEdges = true>
requires concepts::Parseable<Edge> && concepts::HasSource<Edge> && concepts::HasTarget<Edge>
auto parseFromEdgeFile(std::string_view file_path) noexcept
    -> std::optional<graphs::OffsetArray<common::NodeID, Edge, HasForwardEdges, HasBackwardEdges>>
{
    std::ifstream input_file(file_path.data(),
                             std::ios::in);

    if(!input_file) {
        return std::nullopt;
    }

    common::NodeID highest_node{0};
    std::string line;
    std::vector<Edge> edges;

    while(std::getline(input_file, line)) {

        if(line.empty() or line.starts_with('%') or line.starts_with('#')) {
            continue;
        }

        if(auto edge_opt = Edge::parse(line)) {
            auto edge = std::move(edge_opt.value());

            highest_node = util::max(highest_node,
                                     edge.getSrc(),
                                     edge.getTrg());

            edges.emplace_back(std::move(edge));
        }
    }


    return std::optional{
        graphs::OffsetArray<
            common::NodeID,
            Edge,
            HasForwardEdges,
            HasBackwardEdges>{
            highest_node.get(),
            std::move(edges)}};
}


} // namespace parsing
