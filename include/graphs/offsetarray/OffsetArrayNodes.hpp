#pragma once

#include <concepts/NodeLevels.hpp>
#include <concepts/Nodes.hpp>
#include <numeric>
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

    NonTrivialOffsetArrayNodes(std::vector<Node> nodes) noexcept
        : nodes_(std::move(nodes))
    {
        // clang-format off
        static_assert(concepts::HasNodes<NonTrivialOffsetArrayNodes<Graph, Node>>);
        // Nodes have levels -> OffsetarrayNodes are node level writeable
        static_assert(!concepts::HasLevel<Node>
					  || concepts::WriteableNodeLevels<NonTrivialOffsetArrayNodes<Graph, Node>>);
        // clang-format on
    }

    constexpr NonTrivialOffsetArrayNodes(NonTrivialOffsetArrayNodes<Graph, Node> &&) noexcept = default;
    constexpr NonTrivialOffsetArrayNodes(const NonTrivialOffsetArrayNodes<Graph, Node> &) noexcept = default;

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

    // clang-format off
    template<class F>
    [[nodiscard]] auto sortNodesAccordingTo(F&& func) noexcept
	    -> std::vector<common::NodeID>
	requires std::regular_invocable<F, const Graph&>
	&& std::strict_weak_order<std::invoke_result_t<F, const Graph&>,
				        		  common::NodeID,
								  common::NodeID>
    // clang-format on
    {
        const auto &graph = impl();
        const auto order = std::invoke(std::forward<F>(func), graph);

        std::vector permutation(graph.numberOfNodes(),
                                common::NodeID{0});
        std::iota(std::begin(permutation),
                  std::end(permutation),
                  common::NodeID{0});

        std::sort(std::begin(permutation),
                  std::end(permutation),
                  order);

        //copy the permutation here because it gets shuffled by the algorithm
        auto permutation_copy = permutation;

        for(size_t i = 0; i < graph.numberOfNodes; i++) {
            size_t curr = i;
            size_t next = permutation[curr];
            while(next != i) {
                swap(nodes_[curr], nodes_[next]);
                permutation[curr] = curr;
                curr = next;
                next = permutation[next];
            }
            permutation[curr] = curr;
        }

        return permutation_copy;
    }

    // clang-format off
private:
    //crtp helper function
    constexpr auto impl() const noexcept
        -> const Graph &
    {
        return static_cast<const Graph &>(*this);
    }

    std::vector<Node> nodes_;
    // clang-format on
};

template<class Graph, class Node>
class OffsetArrayNodes : public std::conditional_t<std::is_same_v<Node, common::NodeID>,
                                                   TrivialOffsetArrayNodes,
                                                   NonTrivialOffsetArrayNodes<Graph, Node>>
{
};


} // namespace graphs
