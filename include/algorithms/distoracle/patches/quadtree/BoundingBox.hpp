#pragma once

#include <array>
#include <concepts/Nodes.hpp>
#include <optional>
#include <cmath>

namespace algorithms::impl {

class BoundingBox
{
public:
    constexpr BoundingBox(double x_0, double y_0,
                          double x_1, double y_1) noexcept
        : x_0_(x_0), y_0_(y_0), x_1_(x_1), y_1_(y_1) {}

    [[nodiscard]] constexpr auto isInside(double x, double y) const noexcept
        -> bool
    {
        return x > x_0_ and x < x_1_ and y > y_0_ and y < y_1_;
    }

    [[nodiscard]] constexpr auto split() const noexcept
        -> std::array<BoundingBox, 4>
    {
        const auto [center_x, center_y] = center();
        BoundingBox top_left{x_0_, y_0_, center_x, center_y};
        BoundingBox top_right{center_x, y_0_, x_1_, center_y};
        BoundingBox bottom_left{x_0_, center_y, center_x, y_1_};
        BoundingBox bottom_right{center_x, center_y, x_1_, y_1_};

        return std::array{top_left, top_right, bottom_left, bottom_right};
    }

    [[nodiscard]] constexpr auto diameter() const noexcept
        -> double
    {
        return y_1_ - y_0_;
    }

    [[nodiscard]] constexpr auto operator==(const BoundingBox& rhs) const noexcept
        -> bool
    {
        return x_0_ == rhs.x_0_
            and x_1_ == rhs.x_1_
            and y_0_ == rhs.y_0_
            and y_1_ == rhs.y_1_;
    }

    [[nodiscard]] constexpr auto operator<(const BoundingBox& rhs) const noexcept
        -> bool
    {
        return (x_1_ - x_0_) < (rhs.x_1_ - rhs.x_0_);
    }

    [[nodiscard]] constexpr auto center() const noexcept
        -> std::pair<double, double>
    {
        return std::pair{x_1_ / 2 + x_0_,
                         y_1_ / 2 + y_0_};
    }

    [[nodiscard]] constexpr auto centerDistanceTo(const BoundingBox& other) const noexcept
        -> double
    {
        const auto [x1, y1] = center();
        const auto [x2, y2] = other.center();

        return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }


private:
    double x_0_;
    double y_0_;
    double x_1_;
    double y_1_;
};


// clang-format off
template<class Graph>
requires concepts::HasAccessableNodes<Graph> &&
         concepts::HasLatLng<typename Graph::NodeType>
// clang-format on
constexpr auto createBoundBoxFor(const Graph& graph) noexcept
    -> std::optional<BoundingBox>
{
    if(graph.numberOfNodes() == 0) {
        return std::nullopt;
    }

    double x_0 = 0;
    double y_0 = 0;
    double x_1 = std::numeric_limits<double>::max();
    double y_1 = std::numeric_limits<double>::max();

    for(const auto& node : graph.getNodes()) {
        const auto lat = node.getLat();
        const auto lng = node.getLng();

        x_0 = std::min(x_0, lat);
        y_0 = std::min(y_0, lng);
        x_1 = std::max(x_1, lat);
        y_1 = std::max(y_1, lng);
    }

    return BoundingBox{x_0, y_0, x_1, y_1};
}

} // namespace algorithms::impl
