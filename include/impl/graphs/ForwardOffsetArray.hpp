#pragma once

#include <concepts/structures/Edges.hpp>
#include <concepts/structures/ForwardGraph.hpp>
#include <concepts/structures/NodeLevels.hpp>
#include <concepts/structures/Nodes.hpp>
#include <vector>

namespace graphs {

template<class Node, class Edge>
// the edges need to be able to access the target they are pointing to
requires concepts::structures::HasTarget<Edge>
class ForwardOffsetArray
{
    static_assert(concepts::structures::HasEdges<ForwardOffsetArray<Node, Edge>>);
    static_assert(concepts::structures::HasNodes<ForwardOffsetArray<Node, Edge>>);
    static_assert(
        concepts::structures::ForwardGraph<ForwardOffsetArray<Node, Edge>>);
    // if the edges have weights then the graph musst have writeable edge weights
    static_assert(!concepts::structures::HasWeight<Edge> || concepts::structures::WriteableEdgeWeights<ForwardOffsetArray<Node, Edge>>);
    // if the nodes have levels then the graph musst have writeable node levels
    static_assert(!concepts::structures::HasLevel<Node> || concepts::structures::WriteableNodeLevels<ForwardOffsetArray<Node, Edge>>);

public:
    using EdgeType = Edge;
    using NodeType = Node;

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

    constexpr auto edgeExists(common::EdgeID id) const noexcept -> bool
    {
        return id <= numberOfEdges();
    }

    constexpr auto getEdge(common::EdgeID id) const noexcept -> const Edge *
    {
        if(edgeExists(id)) {
            return &forward_edges_[id];
        }
        return nullptr;
    }

    constexpr auto numberOfEdges() const noexcept -> std::size_t
    {
        return forward_edges_.size();
    }

    constexpr auto checkIfEdgeExistsBetween(common::NodeID from,
                                            common::NodeID to) const noexcept
        -> bool
    {
        auto edge_ids = getForwardEdgeBetween(from, to);
        return std::any_of(std::begin(edge_ids), std::end(edge_ids),
                           [&](auto id) { return getEdge(id)->getTrg() == to; });
    }

    constexpr auto getForwardEdgeIDBetween(common::NodeID from,
                                           common::NodeID to) const noexcept
        -> std::optional<common::EdgeID>
    {
        auto edge_ids = getForwardEdgeBetween(from, to);
        auto iter = std::find(std::begin(edge_ids), std::end(edge_ids),
                              [&](auto id) { return getEdge(id)->getTrg() == to; });

        if(iter != std::end(edge_ids)) {
            return getEdge(*iter)->getTrg();
        }

        return std::nullopt;
    }

    constexpr auto getForwardEdgeBetween(common::NodeID from,
                                         common::NodeID to) const noexcept
        -> const Edge *
    {
        auto edge_ids = getForwardEdgeBetween(from, to);
        auto iter = std::find(std::begin(edge_ids), std::end(edge_ids),
                              [&](auto id) { return getEdge(id)->getTrg() == to; });

        if(iter != std::end(edge_ids)) {
            return getEdge(*iter);
        }

        return nullptr;
    }

    constexpr auto getForwardEdgeIDsOf(common::NodeID node) const noexcept
        -> std::span<const common::EdgeID>
    {
        //if the node does not exist, return an empty span
        if(!nodeExists(node)) {
            return std::span<const common::EdgeID>();
        }

        const auto start_offset = forward_offset_[node.get()];
        const auto end_offset = forward_offset_[node.get() + 1];
        const auto *start = &forward_neigbours_[start_offset];
        const auto *end = &forward_neigbours_[end_offset];

        return std::span{start, end};
    }

    constexpr auto getForwardEdgeIDsOf(common::NodeID node) noexcept
        -> std::span<common::EdgeID>
    {
        if(!nodeExists(node)) {
            return std::span<common::EdgeID>();
        }

        const auto start_offset = forward_offset_[node.get()];
        const auto end_offset = forward_offset_[node.get() + 1];
        auto *start = &forward_neigbours_[start_offset];
        const auto *end = &forward_neigbours_[end_offset];

        return std::span{start, end};
    }

    /**
     * this one is a bit tricky,
     * the passed orders needs to be a function of type:
     * `ForwardOffsetArray<Node, Edge> -> (NodeID, NodeID) -> bool`
     * this means it is callable with *this and returns a function
     * `(NodeID, NodeID) -> bool`
     * this function will then be used to sort the ids of the forward edges of all
     * nodes
     */
    // clang-format off
    template<class F>
    constexpr auto sortForwardEdgeIDsAccordingTo(F&& func) noexcept
        -> void
	    requires std::regular_invocable<F, decltype(*this)>
	          && std::strict_weak_order<decltype(func(*this)),
				        			    common::NodeID,
										common::NodeID>
    // clang-format on
    {
        auto order = std::invoke(std::forward<F>(func), *this);

        for(std::size_t n = 0; n < numberOfNodes(); n++) {
            auto ids = getForwardEdgeIDsOf(n);
            std::sort(std::begin(ids), std::end(ids), order);
        }
    }

    // clang-format off
    constexpr auto getEdgeWeight(common::EdgeID id) const noexcept
        -> std::optional<common::EdgeWeight>
	    requires concepts::structures::HasWeight<EdgeType>
    // clang-format on
    {
        if(const auto *edge = getEdge(id)) {
            return edge->getWeight();
        }
        return std::nullopt;
    }

    // clang-format off
    constexpr auto getEdgeWeightUnsafe(common::EdgeID id) const noexcept
        -> common::EdgeWeight
	    requires concepts::structures::HasWeight<EdgeType>
    // clang-format on
    {
        return getEdge(id)->getWeight();
    }

    // clang-format off
    constexpr auto setEdgeWeight(common::EdgeID id, common::EdgeWeight weight) const noexcept
        -> void
	    requires concepts::structures::HasWeight<EdgeType>
    // clang-format on
    {
        if(const auto *edge = getEdge(id)) {
            return edge->setWeight(weight);
        }
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

    // clang-format off
private:
    std::vector<Node> nodes_;
    std::vector<common::EdgeID> forward_neigbours_;
    std::vector<size_t> forward_offset_;
    std::vector<Edge> forward_edges_;

    // clang-format on
};

} // namespace graphs
