
#pragma once

#include <concepts/Parseable.hpp>
#include <fmt/core.h>
#include <fstream>
#include <graphs/offsetarray/OffsetArray.hpp>

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

} // namespace parsing
