#pragma once

#include <concepts/Parseable.hpp>
#include <fstream>
#include <graphs/offsetarray/OffsetArrayBackwardGraph.hpp>
#include <graphs/offsetarray/OffsetArrayForwardGraph.hpp>

namespace graphs {

class LatLngBase
{
public:
    constexpr LatLngBase(common::Latitude lat, common::Longitude lng) noexcept
        : lat_(lat),
          lng_(lng) {}

    constexpr auto getLat() const noexcept
        -> common::Latitude
    {
        return lat_;
    }

    constexpr auto getLng() const noexcept
        -> common::Longitude
    {
        return lng_;
    }

private:
    common::Latitude lat_;
    common::Longitude lng_;
};

} // namespace graphs
