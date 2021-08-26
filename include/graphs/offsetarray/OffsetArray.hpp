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
class OffsetArray : public OffsetArrayNodes<Node>,
                    public OffsetArrayEdges<Edge>,
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
        -> void requires concepts::ForwardGraph<Self>
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
                  "the graph should fullfill the HasNodes concept");

    static_assert(concepts::HasEdges<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "the graph should fullfill the HasEdges concept");

    static_assert(!concepts::HasWeight<Edge>
				  || concepts::WriteableEdgeWeights<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph with edges which fullfill the HasWeight concept should itself fullfill the WriteableEdgeWeights concept");
    static_assert(!concepts::HasLevel<Node>
				  || concepts::WriteableNodeLevels<OffsetArray<Node, Edge, HasForwardEdges, HasBackwardEdges>>,
                  "a graph with nodes which fullfill the HasLevel concept should itself fullfill the WriteableNodeLevels concept");
        // clang-format on
    }

public :

    using NodeType = Node;
    using EdgeType = Edge;

    // OffsetArray(std::vector<Node> nodes,
    //             std::vector<Edge> edges) noexcept
    //     requires HasForwardEdges &&(!HasBackwardEdges)

    //     : OffsetArrayNodes<Node>(std::move(nodes)),
    //       OffsetArrayEdges<Edge>(std::move(edges)),
    //       OffsetArrayForwardGraph<Node, Edge, Self>(){}

    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges &&(!HasForwardEdges)
        : OffsetArrayNodes<Node>(std::move(nodes)),
          OffsetArrayEdges<Edge>(std::move(edges)),
          OffsetArrayBackwardGraph<Node, Edge, Self>()
    {}


    OffsetArray(std::vector<Node> nodes,
                std::vector<Edge> edges) noexcept
        requires HasBackwardEdges && HasForwardEdges
        : OffsetArrayNodes<Node>(std::move(nodes)),
          OffsetArrayEdges<Edge>(std::move(edges)),
          OffsetArrayForwardGraph<Node, Edge, Self>(),
          OffsetArrayBackwardGraph<Node, Edge, Self>()
    {
        checkConcepts();
    }

    OffsetArray(Self&&) noexcept = default;
    OffsetArray(const Self&) noexcept = default;
};

template<class Node, class Edge>
using ForwardOffsetArray = OffsetArray<Node, Edge, true, false>;

template<class Node, class Edge>
using BackwardOffsetArray = OffsetArray<Node, Edge, false, true>;

template<class Node, class Edge>
using BidirectionalOffsetArray = OffsetArray<Node, Edge, true, true>;


} // namespace graphs
