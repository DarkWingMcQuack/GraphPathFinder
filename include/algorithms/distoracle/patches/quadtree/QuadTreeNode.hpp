#pragma once

#include <algorithms/distoracle/patches/quadtree/BoundingBox.hpp>
#include <array>
#include <concepts/Nodes.hpp>
#include <optional>

namespace algorithms::impl {

class QuadTreeNode
{
public:
    constexpr QuadTreeNode(BoundingBox box, std::span<const common::NodeID> elems) noexcept
        : box_(box),
          elements_(elems),
          top_left_child_(nullptr),
          top_right_child_(nullptr),
          bottom_left_child_(nullptr),
          bottom_right_child_(nullptr) {}

    QuadTreeNode(BoundingBox box,
                 std::span<const common::NodeID> elems,
                 std::unique_ptr<QuadTreeNode> top_left_child,
                 std::unique_ptr<QuadTreeNode> top_right_child,
                 std::unique_ptr<QuadTreeNode> bottom_left_child,
                 std::unique_ptr<QuadTreeNode> bottom_right_child) noexcept
        : box_(box),
          elements_(elems),
          top_left_child_(std::move(top_left_child)),
          top_right_child_(std::move(top_right_child)),
          bottom_left_child_(std::move(bottom_left_child)),
          bottom_right_child_(std::move(bottom_right_child)) {}

    [[nodiscard]] constexpr auto operator==(const QuadTreeNode& rhs) const noexcept
        -> bool
    {
        return box_ == rhs.box_;
    }

    [[nodiscard]] constexpr auto getElements() const noexcept
        -> std::span<const common::NodeID>
    {
        return elements_;
    }

    [[nodiscard]] constexpr auto diam() const noexcept
        -> double
    {
        if(elements_.size() <= 1) {
            return 0.0;
        }

        return box_.diameter();
    }

    [[nodiscard]] constexpr auto distanceTo(const QuadTreeNode& other) const noexcept
        -> double
    {
        const auto center_dist = box_.centerDistanceTo(other.box_);
        const auto radius1 = box_.diameter() / 2;
        const auto radius2 = other.box_.diameter() / 2;
        return center_dist - radius1 - radius2;
    }




    [[nodiscard]] constexpr auto empty() const noexcept
        -> bool
    {
        return elements_.empty();
    }

    [[nodiscard]] constexpr auto size() const noexcept
        -> std::size_t
    {
        return elements_.size();
    }

    [[nodiscard]] constexpr auto getBoundingBox() const noexcept
        -> BoundingBox
    {
        return box_;
    }

    [[nodiscard]] constexpr auto getTopLeftChild() const noexcept
        -> const QuadTreeNode*
    {
        return top_left_child_.get();
    }

    [[nodiscard]] constexpr auto getTopRightChild() const noexcept
        -> const QuadTreeNode*
    {
        return top_right_child_.get();
    }

    [[nodiscard]] constexpr auto getBottomLeftChild() const noexcept
        -> const QuadTreeNode*
    {
        return bottom_left_child_.get();
    }

    [[nodiscard]] constexpr auto getBottomRightChild() const noexcept
        -> const QuadTreeNode*
    {
        return bottom_right_child_.get();
    }

    constexpr auto setTopLeftChild(std::unique_ptr<QuadTreeNode>&& child) noexcept
        -> void
    {
        top_left_child_ = std::move(child);
    }

    constexpr auto setTopRightChild(std::unique_ptr<QuadTreeNode>&& child) noexcept
        -> void
    {
        top_right_child_ = std::move(child);
    }

    constexpr auto setBottomLeftChild(std::unique_ptr<QuadTreeNode>&& child) noexcept
        -> void
    {
        bottom_left_child_ = std::move(child);
    }

    constexpr auto setBottomRightChild(std::unique_ptr<QuadTreeNode>&& child) noexcept
        -> void
    {
        bottom_right_child_ = std::move(child);
    }

private:
    BoundingBox box_;
    std::span<const common::NodeID> elements_;

    std::unique_ptr<const QuadTreeNode> top_left_child_;
    std::unique_ptr<const QuadTreeNode> top_right_child_;
    std::unique_ptr<const QuadTreeNode> bottom_left_child_;
    std::unique_ptr<const QuadTreeNode> bottom_right_child_;
};

} // namespace algorithms::impl
