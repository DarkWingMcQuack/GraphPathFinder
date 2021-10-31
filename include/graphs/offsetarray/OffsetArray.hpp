#pragma once

#include <common/EmptyBase.hpp>
#include <concepts/Parseable.hpp>
#include <concepts/Permutable.hpp>
#include <concepts/Sortable.hpp>
#include <graphs/offsetarray/OffsetArrayBackwardGraph.hpp>
#include <graphs/offsetarray/OffsetArrayEdges.hpp>
#include <graphs/offsetarray/OffsetArrayForwardGraph.hpp>
#include <graphs/offsetarray/OffsetArrayNodes.hpp>

namespace graphs {

template<class Node,
         class Edge,
         bool HasForwardEdges = true,
         bool HasBackwardEdges = true>
requires concepts::HasSource<Edge> && concepts::HasTarget<Edge>
class OffsetArray : public OffsetArrayNodes<OffsetArray<Node,
                                                        Edge,
                                                        HasForwardEdges,
                                                        HasBackwardEdges>,
                                            Node>,
                    public OffsetArrayEdges<OffsetArray<Node,
                                                        Edge,
                                                        HasForwardEdges,
                                                        HasBackwardEdges>,
                                            Edge>,
                    public std::conditional_t<HasForwardEdges,
                                              OffsetArrayForwardGraph<Node,
                                                                      Edge,
                                                                      OffsetArray<Node,
                                                                                  Edge,
                                                                                  HasForwardEdges,
                                                                                  HasBackwardEdges>>,
                                              common::EmptyBase1>,
                    public std::conditional_t<HasBackwardEdges,
                                              OffsetArrayBackwardGraph<Node,
                                                                       Edge,
                                                                       OffsetArray<Node,
                                                                                   Edge,
                                                                                   HasForwardEdges,
                                                                                   HasBackwardEdges>>,
                                              common::EmptyBase2>
{

private:
    constexpr auto checkConcepts() const noexcept
        -> void
    {
        static_assert(HasForwardEdges || HasBackwardEdges,
                      "a graph without forward or backward edges is not allowed");

        static_assert(!HasForwardEdges || concepts::ForwardConnections<OffsetArray>,
                      "a graph which has forward edges should fullfill the ForwardGraph concept");

        static_assert(!HasBackwardEdges || concepts::BackwardConnections<OffsetArray>,
                      "a graph which has backward edges should fullfill the BackwardGraph concept");

        static_assert(concepts::HasNodes<OffsetArray>, "every graph should fullfill the HasNodes concept");

        static_assert(!std::is_same_v<Node, common::NodeID> || concepts::HasNontrivialNodes<OffsetArray>,
                      "a graph with non-trivial nodes should fullfill the HasNontrivialNodes concept");

        static_assert(concepts::HasEdges<OffsetArray>,
                      "every graph should fullfill the HasEdges concept");

        static_assert(!concepts::HasWeight<Edge> || concepts::WriteableEdgeWeights<OffsetArray>,
                      "a graph with edges which fullfill the HasWeight concept should itself fullfill the WriteableEdgeWeights concept");
        static_assert(!concepts::HasLevel<Node> || concepts::WriteableNodeLevels<OffsetArray>,
                      "a graph with nodes which fullfill the HasLevel concept should itself fullfill the WriteableNodeLevels concept");

        static_assert(!HasForwardEdges || concepts::SortableForwardConnections<OffsetArray>,
                      "an offsetarray which is a forward graph should also be able to sort the forward connections");

        static_assert(!HasBackwardEdges || concepts::SortableBackwardConnections<OffsetArray>,
                      "an offsetarray which is a backward graph should also be able to sort the backward connections");

        static_assert(!HasForwardEdges || concepts::ForwardConnections<OffsetArray>,
                      "an offsetarray which is a forward graph should also have forward connections");

        static_assert(!HasBackwardEdges || concepts::BackwardConnections<OffsetArray>,
                      "an offsetarray which is a backward graph should also have backward connections");

        static_assert(concepts::EdgesSortable<OffsetArray>, "edges of an offsetarray should be sortable");
        static_assert(concepts::NodesSortable<OffsetArray>, "nodes of an offsetarray should be sortable");
        static_assert(concepts::EdgesPermutable<OffsetArray>, "edges of an offsetarray should be permutable");
        static_assert(concepts::NodesPermutable<OffsetArray>, "nodes of an offsetarray should be permutable");
    }

public:
    using NodeType = Node;
    using EdgeType = Edge;

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)

        : OffsetArrayNodes<OffsetArray, Node>(std::move(nodes)),
          OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)
        && std::is_same_v<Node, common::NodeID>

        : OffsetArrayNodes<OffsetArray, Node>(number_of_nodes),
        OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
        OffsetArrayForwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }


    // currently clang has a bug of not allowing all three constructors at the same time
    // once this is fixed thei check can be omitted
#ifndef __clang__
    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges &&(!HasForwardEdges)
        : OffsetArrayNodes<OffsetArray, Node>(std::move(nodes)),
          OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
          OffsetArrayBackwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges
        &&(!HasForwardEdges)
        && std::is_same_v<Node, common::NodeID>
        : OffsetArrayNodes<OffsetArray, Node>(number_of_nodes),
        OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
        OffsetArrayBackwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }
#endif


    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<OffsetArray, Node>(std::move(nodes)),
          OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, OffsetArray>(),
          OffsetArrayBackwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<OffsetArray, Node>(number_of_nodes),
          OffsetArrayEdges<OffsetArray, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, OffsetArray>(),
          OffsetArrayBackwardGraph<Node, Edge, OffsetArray>()
    {
        checkConcepts();
    }

    constexpr OffsetArray(OffsetArray&&) noexcept = default;
    constexpr OffsetArray(const OffsetArray&) noexcept = default;
    constexpr auto operator=(OffsetArray&&) noexcept -> OffsetArray& = default;
    constexpr auto operator=(const OffsetArray&) noexcept -> OffsetArray& = default;

    // clang-format off
    template<class F>
    auto sortNodesAccordingTo(F&& func) noexcept
	    -> std::pair<std::vector<std::size_t>, std::vector<std::size_t> >
	    requires std::regular_invocable<F, const OffsetArray&>
	          && std::strict_weak_order<std::invoke_result_t<F, const OffsetArray&>,
		        				        common::NodeID,
								        common::NodeID>
    // clang-format on
    {
        const auto number_of_nodes = this->numberOfNodes();
        const auto f = std::invoke(std::forward<F>(func), *this);
        const auto order = [&](const auto lhs, const auto rhs) {
            return f(common::NodeID{lhs}, common::NodeID{rhs});
        };


        std::vector<std::size_t> perm(number_of_nodes);
        std::iota(std::begin(perm), std::end(perm), 0);
        std::sort(std::begin(perm), std::end(perm), order);

        // dont const because it will be moved out of the function
        auto inv_perm = util::inversePermutation(perm);

        applyNodePermutation(perm, inv_perm);

        return std::pair{std::move(perm), std::move(inv_perm)};
    }

    auto applyNodePermutation(std::vector<std::size_t> perm,
                              const std::vector<std::size_t>& inv_perm) noexcept
        -> bool
    {
        const auto number_of_nodes = this->numberOfNodes();
        const auto number_of_edges = this->numberOfEdges();

        if(number_of_nodes != perm.size() or number_of_nodes != inv_perm.size()) {
            return false;
        }

        // apply the permutation to the forward offsetarray
        if constexpr(concepts::ForwardConnections<OffsetArray>) {
            std::vector<size_t> forward_offset(number_of_nodes + 1, 0);
            std::vector<common::EdgeID> forward_neigbours;
            forward_neigbours.reserve(number_of_edges);

            for(size_t i = 0; i < number_of_nodes; i++) {
                common::NodeID n{perm[i]};
                const auto neigs = this->getForwardEdgeIDsOf(n);
                forward_neigbours.insert(std::end(forward_neigbours),
                                         std::begin(neigs),
                                         std::end(neigs));

                forward_offset[i + 1] = forward_neigbours.size();
            }
            this->forward_offset_ = std::move(forward_offset);
            this->forward_neigbours_ = std::move(forward_neigbours);
        }

        // apply the permutation to the backward offsetarray
        if constexpr(concepts::BackwardConnections<OffsetArray>) {
            std::vector<size_t> backward_offset(number_of_nodes + 1, 0);
            std::vector<common::EdgeID> backward_neigbours;
            backward_neigbours.reserve(number_of_edges);


            for(size_t i = 0; i < number_of_nodes; i++) {
                common::NodeID n{perm[i]};
                const auto neigs = this->getBackwardEdgeIDsOf(n);
                backward_neigbours.insert(std::end(backward_neigbours),
                                          std::begin(neigs),
                                          std::end(neigs));

                backward_offset[i + 1] = backward_neigbours.size();
            }
            this->backward_offset_ = std::move(backward_offset);
            this->backward_neigbours_ = std::move(backward_neigbours);
        }

        // update the sources and targets of edges in the graph
        if constexpr(concepts::HasEdges<OffsetArray>) {
            for(auto& e : this->edges_) {

                // update src if available
                if constexpr(concepts::HasSource<EdgeType>) {
                    auto current_src = e.getSrc();
                    auto new_src = common::NodeID{inv_perm[current_src.get()]};
                    e.setSrc(new_src);
                }

                // update trg if available
                if constexpr(concepts::HasTarget<EdgeType>) {
                    auto current_trg = e.getTrg();
                    auto new_trg = common::NodeID{inv_perm[current_trg.get()]};
                    e.setTrg(new_trg);
                }
            }
        }

        // update the nodes_ array if available
        if constexpr(!std::is_same_v<NodeType, common::NodeID>) {
            this->nodes_ = util::applyPermutation(std::move(this->nodes_),
                                                  std::move(perm));
        }

        return true;
    }

    // clang-format off
    template<class F>
    auto sortEdgesAccordingTo(F&& func) noexcept
	    -> std::pair<std::vector<std::size_t>, std::vector<std::size_t>>
	    requires std::regular_invocable<F, const OffsetArray&>
	          && std::strict_weak_order<std::invoke_result_t<F, const OffsetArray&>,
								        common::EdgeID,
								        common::EdgeID>
    // clang-format on
    {
        const auto number_of_edges = this->numberOfEdges();
        const auto f = std::invoke(std::forward<F>(func), *this);
        const auto order = [&](const auto lhs, const auto rhs) {
            return f(common::EdgeID{lhs}, common::EdgeID{rhs});
        };


        std::vector<std::size_t> perm(number_of_edges);
        std::iota(std::begin(perm), std::end(perm), 0);
        std::sort(std::begin(perm), std::end(perm), order);

        auto inv_perm = util::inversePermutation(perm);

        applyEdgePermutation(perm, inv_perm);

        return std::pair{std::move(perm), std::move(inv_perm)};
    }

    auto applyEdgePermutation(std::vector<std::size_t> perm,
                              const std::vector<std::size_t>& inv_perm) noexcept
        -> bool
    {
        const auto number_of_edges = this->numberOfEdges();
        if(number_of_edges != perm.size() or number_of_edges != inv_perm.size()) {
            return false;
        }

        // apply the permutation to the forward connections
        if constexpr(HasForwardEdges) {
            std::transform(std::begin(this->forward_neigbours_),
                           std::end(this->forward_neigbours_),
                           std::begin(this->forward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{inv_perm[id.get()]};
                           });
        }

        // apply the permutation to the backward connections
        if constexpr(HasBackwardEdges) {
            std::transform(std::begin(this->backward_neigbours_),
                           std::end(this->backward_neigbours_),
                           std::begin(this->backward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{inv_perm[id.get()]};
                           });
        }

        this->edges_ = util::applyPermutation(std::move(this->edges_),
                                              std::move(perm));

        // permute the shortcuts
        if constexpr(concepts::CanHaveShortcuts<EdgeType>) {
            for(auto& edge : this->edges_) {
                if(!edge.isShortcut()) {
                    continue;
                }

                const auto [first, second] = edge.getShortcutUnsafe();
                edge.setShortcut(common::EdgeID{inv_perm[first.get()]},
                                 common::EdgeID{inv_perm[second.get()]});
            }
        }

        return true;
    }
};

template<class Node, class Edge>
using ForwardOffsetArray = OffsetArray<Node, Edge, true, false>;

template<class Node, class Edge>
using BackwardOffsetArray = OffsetArray<Node, Edge, false, true>;

template<class Node, class Edge>
using BidirectionalOffsetArray = OffsetArray<Node, Edge, true, true>;


} // namespace graphs
