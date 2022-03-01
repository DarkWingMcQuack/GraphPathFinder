#pragma once

#include <algorithms/distoracle/patches/quadtree/BoundingBox.hpp>
#include <algorithms/distoracle/patches/quadtree/QuadTreeNode.hpp>
#include <concepts/Nodes.hpp>
#include <fmt/core.h>
#include <numeric>
#include <optional>

namespace algorithms {

class QuadTree
{
private:
    // clang-format off
    template<class Graph>
    requires concepts::HasAccessableNodes<Graph> &&
	         concepts::HasNontrivialNodes<Graph> &&
	         concepts::HasLatLng<typename Graph::NodeType>
    // clang-format on
    friend class QuadTreeConstructor;
    friend class WSPD;

    // a quad tree should be created by a QuadTreeConstructor
    QuadTree(impl::QuadTreeNode root,
			 std::vector<common::NodeID> elements) noexcept
        : root_(std::move(root)), elements_(std::move(elements))
    {}

public:
    [[nodiscard]] auto getRoot() const noexcept
        -> const impl::QuadTreeNode&
    {
        return root_;
    }

private:
    impl::QuadTreeNode root_;
    std::vector<common::NodeID> elements_;
};

} // namespace algorithms
