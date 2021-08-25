#pragma once

#include <concepts/NodeLevels.hpp>
#include <concepts/Nodes.hpp>
#include <vector>

namespace graphs {

template<class Node>
class OffsetArrayNodes
{
    static_assert(concepts::structures::HasNodes<OffsetArrayNodes<Node>>);
    // Nodes have levels -> OffsetarrayNodes are node level writeable
    static_assert(!concepts::structures::HasLevel<Node> || concepts::structures::WriteableNodeLevels<OffsetArrayNodes<Node>>);

public:
    using NodeType = Node;

    //TODO: make the ctor constexpr once the compiler supports it
    OffsetArrayNodes(std::vector<Node> nodes) noexcept
        : nodes_(std::move(nodes)) {}

    constexpr auto nodeExists(common::NodeID id) const noexcept -> bool
    {
        return id <= numberOfNodes();
    }

    constexpr auto getNode(common::NodeID id) const noexcept -> const Node *
    {
        if(nodeExists(id)) {
            return &nodes_[id];
        }

        return nullptr;
    }

    constexpr auto numberOfNodes() const noexcept -> std::size_t
    {
        return nodes_.size();
    }

    // clang-format off
    constexpr auto getNodeLevel(common::NodeID id) const noexcept
        -> std::optional<common::NodeLevel>
	    requires concepts::structures::HasLevel<NodeType>
    // clang-format on
    {
        if(const auto *node = getNode(id)) {
            return node->getLevel();
        }

        return std::nullopt;
    }

    // clang-format off
    constexpr auto getNodeLevelUnsafe(common::NodeID id) const noexcept
        -> common::NodeLevel
        requires concepts::structures::HasLevel<NodeType>
    //clang-format on
    {
        return getNode(id)->getLevel();
    }

    // clang-format off
    constexpr auto setNodeLevel(common::NodeID id, common::NodeLevel lvl) noexcept
        -> void
	    requires concepts::structures::HasLevel<NodeType>
    // clang-format on
    {
        if(const auto *node = getNode(id)) {
            return node->setLevel(lvl);
        }
    }

    constexpr auto getNodes() const noexcept
        -> const std::vector<Node> &
    {
        return nodes_;
    }

    // clang-format off
private:
    std::vector<Node> nodes_;
    // clang-format on
};

} // namespace graphs
