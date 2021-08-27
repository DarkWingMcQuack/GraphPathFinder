#pragma once

#include <concepts/NodeLevels.hpp>
#include <concepts/Nodes.hpp>
#include <vector>

namespace graphs {

template<class Node>
class OffsetArrayNodes
{
public:
    using NodeType = Node;

    OffsetArrayNodes(std::vector<Node> nodes) noexcept
        : nodes_(std::move(nodes))
    {
        // clang-format off
        static_assert(concepts::HasNodes<OffsetArrayNodes<Node>>);
        // Nodes have levels -> OffsetarrayNodes are node level writeable
        static_assert(!concepts::HasLevel<Node>
					  || concepts::WriteableNodeLevels<OffsetArrayNodes<Node>>);
        // clang-format on
    }

    OffsetArrayNodes(OffsetArrayNodes<Node> &&) noexcept = default;
    OffsetArrayNodes(const OffsetArrayNodes<Node> &) noexcept = default;

    constexpr auto nodeExists(common::NodeID id) const noexcept -> bool
    {
        return id.get() < numberOfNodes();
    }

    constexpr auto getNode(common::NodeID id) const noexcept -> const Node *
    {
        if(nodeExists(id)) {
            return &nodes_[id.get()];
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
	    requires concepts::HasLevel<NodeType>
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
        requires concepts::HasLevel<NodeType>
    //clang-format on
    {
        return getNode(id)->getLevel();
    }

    // clang-format off
    constexpr auto setNodeLevel(common::NodeID id, common::NodeLevel lvl) noexcept
        -> void
	    requires concepts::HasLevel<NodeType>
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
