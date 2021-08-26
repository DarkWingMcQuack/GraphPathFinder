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

    constexpr LatLngBase(LatLngBase&&) noexcept = default;
    constexpr LatLngBase(const LatLngBase&) noexcept = default;

    constexpr auto operator=(const LatLngBase&) noexcept
        -> LatLngBase& = default;
    constexpr auto operator=(LatLngBase&&) noexcept
        -> LatLngBase& = default;

    constexpr auto operator==(const LatLngBase&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const LatLngBase&) const noexcept
        -> bool = default;

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
