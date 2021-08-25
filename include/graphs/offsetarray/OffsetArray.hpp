#pragma once

#include <common/EmptyBase.hpp>
#include <concepts/Parseable.hpp>
#include <graphs/offsetarray/OffsetArrayBackwardGraph.hpp>
#include <graphs/offsetarray/OffsetArrayForwardGraph.hpp>

namespace graphs {


template<class Node,
         class Edge,
         bool HasForwardEdges = true,
         bool HasBackwardEdges = true>
//currently edges need to have source and target available
//this can be relaxed in the future such that OffsetArrayForwardGraph do not require the
//HasSource concept, but currently this is needed in the ctor of OffsetArrayForwardGraph
requires concepts::structures::HasSource<Edge> && concepts::structures::HasTarget<Edge>
class OffsetArray : public std::conditional_t<HasForwardEdges,
                                              OffsetArrayForwardGraph<Node, Edge>,
                                              common::EmptyBase1>,
                    public std::conditional_t<HasBackwardEdges,
                                              OffsetArrayBackwardGraph<Node, Edge>,
                                              common::EmptyBase2>
{
    // clang-format off
    static_assert(HasForwardEdges || HasBackwardEdges,
				  "a graph without forward or backward edges is not allowed");

    static_assert(!HasForwardEdges
				  || concepts::structures::ForwardGraph<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph which has forward edges should fullfill the ForwardGraph concept");

    static_assert(!HasBackwardEdges
				  || concepts::structures::BackwardGraph<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph which has backward edges should fullfill the BackwardGraph concept");

    static_assert(concepts::structures::HasNodes<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "the graph should fullfill the HasNodes concept");

    static_assert(concepts::structures::HasEdges<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "the graph should fullfill the HasEdges concept");

    static_assert(!concepts::structures::HasWeight<Edge>
				  || concepts::structures::WriteableEdgeWeights<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph with edges which fullfill the HasWeight concept should itself fullfill the WriteableEdgeWeights concept");
    static_assert(!concepts::structures::HasLevel<Node>
				  || concepts::structures::WriteableNodeLevels<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph with nodes which fullfill the HasLevel concept should itself fullfill the WriteableNodeLevels concept");
    // clang-format on

public:
    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasForwardEdges &&(!HasBackwardEdges)
        : OffsetArrayNodes<Node>(std::move(nodes)),
          OffsetArrayEdges<Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge>(this->getNodes(),
                                              this->getEdges()) {}

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges &&(!HasForwardEdges)
        : OffsetArrayNodes<Node>(std::move(nodes)),
          OffsetArrayEdges<Edge>(std::move(edges)),
          OffsetArrayBackwardGraph<Node, Edge>(this->getNodes(),
                                               this->getEdges()) {}

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<Node>(std::move(nodes)),
          OffsetArrayEdges<Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge>(this->getNodes(),
                                              this->getEdges()),
          OffsetArrayBackwardGraph<Node, Edge>(this->getNodes(),
                                               this->getEdges()) {}
};

template<class Node, class Edge>
using ForwardOffsetArray = OffsetArray<Node, Edge, true, false>;

template<class Node, class Edge>
using BackwardOffsetArray = OffsetArray<Node, Edge, false, true>;

template<class Node, class Edge>
using BidirectionalOffsetArray = OffsetArray<Node, Edge, true, true>;


} // namespace graphs
