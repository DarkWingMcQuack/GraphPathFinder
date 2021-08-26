#pragma once

#include <common/BasicGraphTypes.hpp>
#include <concepts/Parseable.hpp>
#include <fstream>

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
