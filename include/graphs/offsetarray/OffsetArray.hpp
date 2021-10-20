#pragma once

#include <common/EmptyBase.hpp>
#include <concepts/Parseable.hpp>
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
    using Self = OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>;

private:
    constexpr auto checkConcepts() const noexcept
        -> void
    {
        // clang-format off
        static_assert(HasForwardEdges || HasBackwardEdges,
                      "a graph without forward or backward edges is not allowed");

        static_assert(!HasForwardEdges
                      || concepts::ForwardGraph<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "a graph which has forward edges should fullfill the ForwardGraph concept");


        static_assert(!HasBackwardEdges
                      || concepts::BackwardGraph<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "a graph which has backward edges should fullfill the BackwardGraph concept");

        static_assert(concepts::HasNodes<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "every graph should fullfill the HasNodes concept");

        static_assert(!std::is_same_v<Node, common::NodeID>
                      || concepts::HasNontrivialNodes<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "a graph with non-trivial nodes should fullfill the HasNontrivialNodes concept");

        static_assert(concepts::HasEdges<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "every graph should fullfill the HasEdges concept");

        static_assert(!concepts::HasWeight<Edge>
					  || concepts::WriteableEdgeWeights<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "a graph with edges which fullfill the HasWeight concept should itself fullfill the WriteableEdgeWeights concept");
        static_assert(!concepts::HasLevel<Node>
					  || concepts::WriteableNodeLevels<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                      "a graph with nodes which fullfill the HasLevel concept should itself fullfill the WriteableNodeLevels concept");
        // clang-format on
    }

public:
    using NodeType = Node;
    using EdgeType = Edge;

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)

        : OffsetArrayNodes<Self, Node>(std::move(nodes)),
          OffsetArrayEdges<Self, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges
        &&(!HasBackwardEdges)
        && std::is_same_v<Node, common::NodeID>

        : OffsetArrayNodes<Self, Node>(number_of_nodes),
        OffsetArrayEdges<Self, Edge>(std::move(edges)),
        OffsetArrayForwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }


    //currently clang has a bug of not allowing all three constructors at the same time
    //once this is fixed thei check can be omitted
#ifndef __clang__
    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges &&(!HasForwardEdges)
        : OffsetArrayNodes<Self, Node>(std::move(nodes)),
          OffsetArrayEdges<Self, Edge>(std::move(edges)),
          OffsetArrayBackwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges
        &&(!HasForwardEdges)
        && std::is_same_v<Node, common::NodeID>
        : OffsetArrayNodes<Self, Node>(number_of_nodes),
        OffsetArrayEdges<Self, Edge>(std::move(edges)),
        OffsetArrayBackwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }
#endif


    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<Self, Node>(std::move(nodes)),
          OffsetArrayEdges<Self, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, Self>(),
          OffsetArrayBackwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }

    OffsetArray(std::size_t number_of_nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<Self, Node>(number_of_nodes),
          OffsetArrayEdges<Self, Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, Self>(),
          OffsetArrayBackwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }

    OffsetArray(Self&&) noexcept = default;
    OffsetArray(const Self&) noexcept = default;
    auto operator=(Self&&) noexcept -> OffsetArray& = default;
    auto operator=(const Self&) noexcept -> OffsetArray& = default;

    // clang-format off
    template<class F>
    auto sortNodesAccordingTo(F&& func) noexcept
	  -> std::vector<std::size_t>
	requires std::regular_invocable<F, const Self&>
	      && std::strict_weak_order<std::invoke_result_t<F, const Self&>,
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

        auto inv_perm = util::inversePermutation(perm);

        //TODO: permutate the offset array
        auto number_of_nodes = this->numberOfNodes();
        auto number_of_edges = this->numberOfEdges();

        //apply the permutation to the forward offsetarray
        if constexpr(concepts::ForwardGraph<Self>) {
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
        if constexpr(concepts::BackwardGraph<Self>) {
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
        if constexpr(concepts::HasEdges<Self>) {
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
	requires std::regular_invocable<F, const Self&>
	      && std::strict_weak_order<std::invoke_result_t<F, const Self&>,
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
        if constexpr(concepts::ForwardGraph<Self>) {
            std::transform(std::begin(this->forward_neigbours_),
                           std::end(this->forward_neigbours_),
                           std::begin(this->forward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{permutation[id.get()]};
                           });
        }

        //apply the permutation to the backward connections
        if constexpr(concepts::BackwardGraph<Self>) {
            std::transform(std::begin(this->backward_neigbours_),
                           std::end(this->backward_neigbours_),
                           std::begin(this->backward_neigbours_),
                           [&](auto id) {
                               return common::EdgeID{permutation[id.get()]};
                           });
        }

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
