#pragma once

#include <concepts/NodeLevels.hpp>
#include <concepts/Nodes.hpp>
#include <numeric>
#include <utils/Permutation.hpp>
#include <vector>

namespace graphs {

class TrivialOffsetArrayNodes
{
    using NodeType = common::NodeID;

    constexpr TrivialOffsetArrayNodes(std::size_t number_of_nodes) noexcept
        : number_of_nodes_(number_of_nodes) {}

    constexpr TrivialOffsetArrayNodes(TrivialOffsetArrayNodes &&) noexcept = default;
    constexpr TrivialOffsetArrayNodes(const TrivialOffsetArrayNodes &) noexcept = default;

    constexpr auto operator=(TrivialOffsetArrayNodes &&) noexcept
        -> TrivialOffsetArrayNodes & = default;
    constexpr auto operator=(const TrivialOffsetArrayNodes &) noexcept
        -> TrivialOffsetArrayNodes & = default;

    constexpr auto nodeExists(common::NodeID id) const noexcept -> bool
    {
        return id.get() < numberOfNodes();
    }

    constexpr auto numberOfNodes() const noexcept -> std::size_t
    {
        return number_of_nodes_;
    }

private:
    std::size_t number_of_nodes_;
};

// clang-format off
template<class Graph, class Node>
requires(!std::is_same_v<Node, common::NodeID>)
class NonTrivialOffsetArrayNodes
// clang-format on
{
public:
    using NodeType = Node;

    constexpr NonTrivialOffsetArrayNodes(std::vector<Node> nodes) noexcept
        : nodes_(std::move(nodes))
    {
        static_assert(concepts::HasNodes<NonTrivialOffsetArrayNodes>);
        // Nodes have levels -> OffsetarrayNodes are node level writeable
        static_assert(!concepts::HasLevel<Node> || concepts::WriteableNodeLevels<NonTrivialOffsetArrayNodes>);
    }

    constexpr NonTrivialOffsetArrayNodes(NonTrivialOffsetArrayNodes &&) noexcept = default;
    constexpr NonTrivialOffsetArrayNodes(const NonTrivialOffsetArrayNodes &) noexcept = default;

    constexpr auto operator=(NonTrivialOffsetArrayNodes &&) noexcept
        -> NonTrivialOffsetArrayNodes & = default;

    constexpr auto operator=(const NonTrivialOffsetArrayNodes &) noexcept
        -> NonTrivialOffsetArrayNodes & = default;

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
            return node->getLvl();
        }

        return std::nullopt;
    }

    // clang-format off
    constexpr auto getNodeLevelUnsafe(common::NodeID id) const noexcept
        -> common::NodeLevel
        requires concepts::HasLevel<NodeType>
    //clang-format on
    {
        return getNode(id)->getLvl();
    }

    // clang-format off
    constexpr auto setNodeLevel(common::NodeID id, common::NodeLevel lvl) noexcept
        -> void
	    requires concepts::HasLevel<NodeType>
    // clang-format on
    {
        if(const auto *node = getNode(id)) {
            return node->setLvl(lvl);
        }
    }

    constexpr auto getNodes() const noexcept
        -> std::span<const Node>
    {
        return nodes_;
    }


    // clang-format off
private:
    friend Graph;

    std::vector<Node> nodes_;
    // clang-format on
};

template<class Graph, class Node>
class OffsetArrayNodes : public std::conditional_t<std::is_same_v<Node, common::NodeID>,
                                                   TrivialOffsetArrayNodes,
                                                   NonTrivialOffsetArrayNodes<Graph, Node>>
{
public:
    constexpr OffsetArrayNodes(std::vector<Node> nodes) noexcept
        requires(!std::is_same_v<Node, common::NodeID>)
        : NonTrivialOffsetArrayNodes<Graph, Node>(std::move(nodes)) {}
};


} // namespace graphs
