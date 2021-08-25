#pragma once

#include <concepts/Parseable.hpp>
#include <fstream>
#include <graphs/offsetarray/OffsetArrayBackwardGraph.hpp>
#include <graphs/offsetarray/OffsetArrayForwardGraph.hpp>

namespace graphs {

class LatLngBase
{
public:
    constexpr LatLngBase(double lat, double lng) noexcept
        : lat_(lat),
          lng_(lng) {}

    constexpr auto getLat() const noexcept
        -> double
    {
        return lat_;
    }

    constexpr auto getLng() const noexcept
        -> double
    {
        return lng_;
    }

private:
    double lat_;
    double lng_;
};

} // namespace graphs
