#pragma once

#include <common/BasicGraphTypes.hpp>
#include <optional>
#include <vector>

namespace graphs {

class Path
{
public:
    Path(std::vector<common::NodeID> nodes,
         common::Weight cost)
        : nodes_(std::move(nodes)),
          cost_(cost) {}

    Path(const Path&) noexcept = default;
    Path(Path&&) noexcept = default;
    auto operator=(const Path&) noexcept
        -> Path& = default;
    auto operator=(Path&&) noexcept
        -> Path& = default;

    constexpr auto operator<(const Path& other) const noexcept
        -> bool
    {
        return cost_ < other.cost_;
    }

    constexpr auto operator>(const Path& other) const noexcept
        -> bool
    {
        return cost_ > other.cost_;
    }

    constexpr auto operator==(const Path& other) const noexcept
        -> bool
    {
        return nodes_ == other.nodes_;
    }

    constexpr auto operator!=(const Path& other) const noexcept
        -> bool
    {
        return nodes_ != other.nodes_;
    }

    [[nodiscard]] constexpr auto getNumberOfNodes() const noexcept
        -> std::size_t
    {
        return nodes_.size();
    }

    [[nodiscard]] constexpr auto getCost() const noexcept
        -> common::Weight
    {
        return cost_;
    }

    [[nodiscard]] constexpr auto getSource() const noexcept
        -> std::optional<common::NodeID>
    {
        if(nodes_.empty()) {
            return std::nullopt;
        }

        return nodes_.front();
    }

    [[nodiscard]] constexpr auto getTarget() const noexcept
        -> std::optional<common::NodeID>
    {
        if(nodes_.empty()) {
            return std::nullopt;
        }

        return nodes_.back();
    }


private:
    std::vector<common::NodeID> nodes_;
    common::Weight cost_;
};

} // namespace graphs
