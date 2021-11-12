#pragma once

#include <common/BasicGraphTypes.hpp>
#include <optional>
#include <vector>

namespace graphs {

class Path
{
public:
    Path(std::vector<common::NodeID> nodes, common::Weight cost)
        : nodes_(std::move(nodes)), cost_(cost) {}

    Path(const Path&) noexcept = default;
    Path(Path&&) noexcept = default;
    auto operator=(const Path&) noexcept -> Path& = default;
    auto operator=(Path&&) noexcept -> Path& = default;

    [[nodiscard]] constexpr auto operator<(const Path& other) const noexcept
        -> bool
    {
        return cost_ < other.cost_;
    }

    [[nodiscard]] constexpr auto operator>(const Path& other) const noexcept
        -> bool
    {
        return cost_ > other.cost_;
    }

    [[nodiscard]] auto operator==(const Path& other) const noexcept
        -> bool
    {
        return nodes_ == other.nodes_;
    }

    [[nodiscard]] auto operator[](int idx) const noexcept
        -> const common::NodeID&
    {
        return nodes_[idx];
    }


    [[nodiscard]] auto operator!=(const Path& other) const noexcept
        -> bool
    {
        return nodes_ != other.nodes_;
    }

    [[nodiscard]] auto getNumberOfNodes() const noexcept
        -> std::size_t
    {
        return nodes_.size();
    }

    [[nodiscard]] constexpr auto getCost() const noexcept
        -> common::Weight
    {
        return cost_;
    }

    [[nodiscard]] auto getSource() const noexcept
        -> std::optional<common::NodeID>
    {
        if(nodes_.empty()) {
            return std::nullopt;
        }

        return nodes_.front();
    }

    [[nodiscard]] auto getTarget() const noexcept
        -> std::optional<common::NodeID>
    {
        if(nodes_.empty()) {
            return std::nullopt;
        }

        return nodes_.back();
    }

    [[nodiscard]] static auto empty() noexcept
        -> Path
    {
        return Path{std::vector<common::NodeID>{}, common::Weight{0}};
    }


private:
    std::vector<common::NodeID> nodes_;
    common::Weight cost_;
};

} // namespace graphs
