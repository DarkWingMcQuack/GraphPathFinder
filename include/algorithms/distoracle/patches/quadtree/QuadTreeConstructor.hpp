#pragma once

#include <algorithms/distoracle/patches/quadtree/BoundingBox.hpp>
#include <algorithms/distoracle/patches/quadtree/QuadTree.hpp>
#include <algorithms/distoracle/patches/quadtree/QuadTreeNode.hpp>
#include <concepts/Nodes.hpp>
#include <optional>
#include <vector>

namespace algorithms {

// clang-format off
template<class Graph>
requires concepts::HasAccessableNodes<Graph> &&
         concepts::HasNontrivialNodes<Graph> &&
         concepts::HasLatLng<typename Graph::NodeType>
// clang-format on
class QuadTreeConstructor
{
public:
    QuadTreeConstructor(const Graph& graph) noexcept
        : graph_(graph)
    {
        nodes_.reserve(graph_.numberOfNodes());
        std::iota(std::begin(nodes_), std::end(nodes_), common::NodeID{0});
    }

    [[nodiscard]] auto constuctQuadTree() && noexcept
        -> std::optional<QuadTree>
    {
        if(graph_.numberOfNodes() == 0) {
            return std::nullopt;
        }

        auto root_box = impl::createBoundBoxFor(graph_);
        auto root_node = *recursiveNodeConstrucion(root_box, nodes_);

        return QuadTree{std::move(root_node), std::move(nodes_)};
    }

private:
    [[nodiscard]] auto recursiveNodeConstrucion(const impl::BoundingBox& box,
                                                const std::span<common::NodeID>& span) noexcept
        -> std::unique_ptr<impl::QuadTreeNode>
    {
        if(span.size() < 2) {
            return std::make_unique<impl::QuadTreeNode>(box, span);
        }

        const auto [tl_box, tr_box, bl_box, br_box] = box.split();
        const auto [tl_end, tr_end, bl_end] = partition(box, span);

        std::span tl_span{std::begin(span), tl_end};
        std::span tr_span{tl_end, tr_end};
        std::span bl_span{tr_end, bl_end};
        std::span br_span{bl_end, std::end(span)};

        auto top_left = recursiveNodeConstrucion(tl_box, tl_span);
        auto top_right = recursiveNodeConstrucion(tr_box, tr_span);
        auto bottom_left = recursiveNodeConstrucion(bl_box, bl_span);
        auto bottom_right = recursiveNodeConstrucion(br_box, br_span);

        return std::make_unique<impl::QuadTreeNode>(box, span,
                                                    std::move(top_left),
                                                    std::move(top_right),
                                                    std::move(bottom_left),
                                                    std::move(bottom_right));
    }

    [[nodiscard]] auto partition(const impl::BoundingBox& box,
                                 const std::span<common::NodeID>& span) noexcept
    {
        const auto [tl_box, tr_box, bl_box, br_box] = box.split();
        const auto tr_end = std::partition(
            std::begin(span), std::end(span),
            [tl_box = tl_box, tr_box = tr_box, this](const auto& n) {
				const auto[lat, lng] = getLatLng(n);
                return tl_box.isInside(lat, lng) or tr_box.isInside(lat, lng);
            });

        const auto tl_end = std::partition(
            std::begin(span), std::end(tr_end),
            [tl_box = tl_box, this](const auto& n) {
				const auto[lat, lng] = getLatLng(n);
                return tl_box.isInside(lat, lng);
            });

        const auto bl_end = std::partition(
            std::begin(tr_end), std::end(span),
            [bl_box = bl_box, this](const auto& n) {
				const auto[lat, lng] = getLatLng(n);
                return bl_box.isInside(lat, lng);
            });

        return std::tuple{tl_end, tr_end, bl_end};
    }

    [[nodiscard]] auto getLatLng(common::NodeID n) const noexcept
        -> std::pair<double, double>
    {
        const auto lat = graph_.getNode(n)->getLat();
        const auto lng = graph_.getNode(n)->getLng();
        return std::pair{lat, lng};
    }

private:
    const Graph& graph_;
    std::vector<common::NodeID> nodes_;
};
} // namespace algorithms
