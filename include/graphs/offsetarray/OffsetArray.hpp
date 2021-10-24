#pragma once

#include <common/EmptyBase.hpp>
#include <concepts/Parseable.hpp>
#include <concepts/SortableGraph.hpp>
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
    using ThisType = OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>;

private:
    constexpr auto checkConcepts() const noexcept
        -> void
    {
        static_assert(HasForwardEdges || HasBackwardEdges,
                      "a graph without forward or backward edges is not allowed");

        static_assert(!HasForwardEdges || concepts::ForwardConnections<ThisType>,
                      "a graph which has forward edges should fullfill the ForwardGraph concept");

        static_assert(!HasBackwardEdges || concepts::BackwardConnections<ThisType>,
                      "a graph which has backward edges should fullfill the BackwardGraph concept");

        static_assert(concepts::HasNodes<ThisType>, "every graph should fullfill the HasNodes concept");

        static_assert(!std::is_same_v<Node, common::NodeID> || concepts::HasNontrivialNodes<ThisType>,
                      "a graph with non-trivial nodes should fullfill the HasNontrivialNodes concept");

        static_assert(concepts::HasEdges<ThisType>,
                      "every graph should fullfill the HasEdges concept");

        static_assert(!concepts::HasWeight<Edge> || concepts::WriteableEdgeWeights<ThisType>,
                      "a graph with edges which fullfill the HasWeight concept should itself fullfill the WriteableEdgeWeights concept");
        static_assert(!concepts::HasLevel<Node> || concepts::WriteableNodeLevels<ThisType>,
                      "a graph with nodes which fullfill the HasLevel concept should itself fullfill the WriteableNodeLevels concept");

        static_assert(!HasForwardEdges || concepts::SortableForwardGraph<ThisType>,
                      "an offsetarray which is a forward graph should also be able to sort the forward connections");

        static_assert(!HasBackwardEdges || concepts::SortableBackwardGraph<ThisType>,
                      "an offsetarray which is a backward graph should also be able to sort the forward connections");

        static_assert(concepts::EdgesSortable<ThisType>, "edges of an offsetarray should be sortable");
        static_assert(concepts::NodesSortable<ThisType>, "nodes of an offsetarray should be sortable");
    }

public:
    using NodeType = Node;
    using EdgeType = Edge;

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)

        : OffsetArrayNodes<ThisType, Node>(std::move(nodes)),
          OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)
        && std::is_same_v<Node, common::NodeID>

        : OffsetArrayNodes<ThisType, Node>(number_of_nodes),
        OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
        OffsetArrayForwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }


    //currently clang has a bug of not allowing all three constructors at the same time
    //once this is fixed thei check can be omitted
#ifndef __clang__
    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges &&(!HasForwardEdges)
        : OffsetArrayNodes<ThisType, Node>(std::move(nodes)),
          OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
          OffsetArrayBackwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges
        &&(!HasForwardEdges)
        && std::is_same_v<Node, common::NodeID>
        : OffsetArrayNodes<ThisType, Node>(number_of_nodes),
        OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
        OffsetArrayBackwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }
#endif


    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<ThisType, Node>(std::move(nodes)),
          OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, ThisType>(),
          OffsetArrayBackwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<ThisType, Node>(number_of_nodes),
          OffsetArrayEdges<ThisType, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, ThisType>(),
          OffsetArrayBackwardGraph<Node, Edge, ThisType>()
    {
        checkConcepts();
    }

    constexpr OffsetArray(ThisType&&) noexcept = default;
    constexpr OffsetArray(const ThisType&) noexcept = default;
    constexpr auto operator=(ThisType&&) noexcept -> OffsetArray& = default;
    constexpr auto operator=(const ThisType&) noexcept -> OffsetArray& = default;

    // clang-format off
    template<class F>
    auto sortNodesAccordingTo(F&& func) noexcept
	  -> std::vector<std::size_t>
	  requires std::regular_invocable<F, const ThisType&>
	  && std::strict_weak_order<std::invoke_result_t<F, const ThisType&>,
								common::NodeID,
								common::NodeID>
    // clang-format on
    {
        const auto order = std::invoke(std::forward<F>(func), *this);

        std::vector<std::size_t> perm(this->numberOfNodes());
        std::iota(std::begin(perm),
                  std::end(perm),
                  0);

        std::sort(std::begin(perm),
                  std::end(perm),
                  order);

        const auto inv_perm = util::inversePermutation(perm);
        const auto number_of_nodes = this->numberOfNodes();
        const auto number_of_edges = this->numberOfEdges();

        //apply the permutation to the forward offsetarray
        if constexpr(concepts::ForwardConnections<ThisType>) {
            std::vector<size_t> forward_offset(number_of_nodes + 1, 0);
            std::vector<common::EdgeID> forward_neigbours;
            forward_neigbours.reserve(number_of_edges);

            for(size_t i = 0; i < number_of_nodes; i++) {
                common::NodeID n{inv_perm[i]};
                auto neigs = this->getForwardEdgeIDsOf(n);

                forward_neigbours.insert(std::end(forward_neigbours),
                                         std::begin(neigs),
                                         std::end(neigs));

                forward_offset[i + 1] = forward_neigbours.size();
            }
            this->forward_offset_ = std::move(forward_offset);
            this->forward_neigbours_ = std::move(forward_neigbours);
        }

        //apply the permutation to the backward offsetarray
        if constexpr(concepts::BackwardConnections<ThisType>) {
            std::vector<size_t> backward_offset(number_of_nodes + 1, 0);
            std::vector<common::EdgeID> backward_neigbours;
            backward_neigbours.reserve(number_of_edges);

            for(size_t i = 0; i < number_of_nodes; i++) {
                common::NodeID n{inv_perm[i]};
                auto neigs = this->getBackwardEdgeIDsOf(n);

                backward_neigbours.insert(std::end(backward_neigbours),
                                          std::begin(neigs),
                                          std::end(neigs));

                backward_offset[i + 1] = backward_neigbours.size();
            }
            this->backward_offset_ = std::move(backward_offset);
            this->backward_neigbours_ = std::move(backward_neigbours);
        }

        //update the sources and targets of edges in the graph
        if constexpr(concepts::HasEdges<ThisType>) {
            for(auto& e : this->edges_) {

                //update src if available
                if constexpr(concepts::HasSource<EdgeType>) {
                    auto current = e.getSrc();
                    auto new_src = common::NodeID{perm[e.getSrc().get()]};
                    e.setSrc(new_src);
                }

                //update trg if available
                if constexpr(concepts::HasTarget<EdgeType>) {
                    auto current = e.getSrc();
                    auto new_trg = common::NodeID{perm[e.getTrg().get()]};
                    e.setTrg(new_trg);
                }
            }
        }

        //update the nodes_ array if available
        if constexpr(!std::is_same_v<NodeType, common::NodeID>) {
            //perm is geting copied here because it will be consumed by the
            //applypermutation function
            this->nodes_ = util::applyPermutation(std::move(this->nodes_),
                                                  perm);
        }

        return perm;
    }

    // clang-format off
    template<class F>
    auto sortEdgesAccordingTo(F&& func) noexcept
	  -> std::vector<std::size_t>
	  requires std::regular_invocable<F, const ThisType&>
	  && std::strict_weak_order<std::invoke_result_t<F, const ThisType&>,
								common::EdgeID,
								common::EdgeID>
    // clang-format on
    {
        const auto order = std::invoke(std::forward<F>(func), *this);

        std::vector<std::size_t> permutation(this->numberOfEdges());
        std::iota(std::begin(permutation),
                  std::end(permutation),
                  0);


        //sort the edgeids according to the given order
        std::sort(std::begin(permutation),
                  std::end(permutation),
                  order);

        //apply the permutation to the forward connections
        if constexpr(concepts::ForwardConnections<ThisType>) {
            std::transform(std::begin(this->forward_neigbours_),
                           std::end(this->forward_neigbours_),
                           std::begin(this->forward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{permutation[id.get()]};
                           });
        }

        //apply the permutation to the backward connections
        if constexpr(concepts::BackwardConnections<ThisType>) {
            std::transform(std::begin(this->backward_neigbours_),
                           std::end(this->backward_neigbours_),
                           std::begin(this->backward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{permutation[id.get()]};
                           });
        }

        //permutation will get copied into applypermutation because it will be consumed
        this->edges = util::applyPermutation(std::move(this->edges_),
                                             permutation);

        return permutation;
    }
};

template<class Node, class Edge>
using ForwardOffsetArray = OffsetArray<Node, Edge, true, false>;

template<class Node, class Edge>
using BackwardOffsetArray = OffsetArray<Node, Edge, false, true>;

template<class Node, class Edge>
using BidirectionalOffsetArray = OffsetArray<Node, Edge, true, true>;


} // namespace graphs
