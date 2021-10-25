#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Parsing.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/common/LevelBase.hpp>
#include <graphs/nodes/LatLngBase.hpp>
#include <string_view>

namespace graphs {


template<bool HasLevel>
class FMINode : public LatLngBase,
                public std::conditional_t<HasLevel,
                                          LevelBase,
                                          common::EmptyBase1>
{
public:
    constexpr FMINode(common::NodeID id2,
                      common::Latitude lat,
                      common::Longitude lng,
                      common::Elevation ele) noexcept
        requires(!HasLevel)
        : LatLngBase(lat, lng),
          id2_(id2),
          elev_(ele) {}

    constexpr FMINode(common::NodeID id2,
                      common::Latitude lat,
                      common::Longitude lng,
                      common::Elevation ele,
                      common::NodeLevel lvl) noexcept
        requires HasLevel
        : LatLngBase(lat, lng),
          LevelBase(lvl),
          id2_(id2),
          elev_(ele) {}

    constexpr FMINode(const FMINode<HasLevel>&) noexcept = default;
    constexpr FMINode(FMINode<HasLevel>&&) noexcept = default;

    constexpr auto operator=(const FMINode<HasLevel>&) noexcept
        -> FMINode<HasLevel>& = default;
    constexpr auto operator=(FMINode<HasLevel>&&) noexcept
        -> FMINode<HasLevel>& = default;

    constexpr auto operator==(const FMINode<HasLevel>&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const FMINode<HasLevel>&) const noexcept
        -> bool = default;

    constexpr auto getID2() const noexcept
        -> common::NodeID
    {
        return id2_;
    }

    constexpr auto getElevation() const noexcept
        -> common::Elevation
    {
        return elev_;
    }

    // clang-format off
    static auto parse(std::string_view str) noexcept
        -> std::optional<FMINode<HasLevel>>
	    requires HasLevel
    // clang-format on
    {
        const auto [id1_sv, id2_sv, lat_sv, lng_sv, elev_sv, lvl_sv] = common::extractFirstN<6>(str, " ");

        const auto id1_opt = common::to<common::NodeID>(id1_sv);
        const auto id2_opt = common::to<common::NodeID>(id2_sv);
        const auto lat_opt = common::to<common::Latitude>(lat_sv);
        const auto lng_opt = common::to<common::Longitude>(lng_sv);
        const auto elev_opt = common::to<common::Elevation>(elev_sv);
        const auto lvl_opt = common::to<common::NodeLevel>(lvl_sv);

        if(!id1_opt or !id2_opt or !lat_opt or !lng_opt or !elev_opt or !lvl_opt) {
            return std::nullopt;
        }

        return FMINode<HasLevel>{
            id2_opt.value(),
            lat_opt.value(),
            lng_opt.value(),
            elev_opt.value(),
            lvl_opt.value()};
    }

    // clang-format off
    static auto parse(std::string_view str) noexcept
        -> std::optional<FMINode<HasLevel>>
	    requires (!HasLevel)
    // clang-format on
    {
        const auto [id1_sv, id2_sv, lat_sv, lng_sv, elev_sv] = common::extractFirstN<5>(str, " ");

        const auto id2_opt = common::to<common::NodeID>(id2_sv);
        const auto id1_opt = common::to<common::NodeID>(id1_sv);
        const auto lat_opt = common::to<common::Latitude>(lat_sv);
        const auto lng_opt = common::to<common::Longitude>(lng_sv);
        const auto elev_opt = common::to<common::Elevation>(elev_sv);

        if(!id1_opt or !lat_opt or !lng_opt or !id2_opt or !elev_opt) {
            return std::nullopt;
        }

        return FMINode<HasLevel>{
            id2_opt.value(),
            lat_opt.value(),
            lng_opt.value(),
            elev_opt.value()};
    }

    // clang-format off
private:
    common::NodeID id2_;
    common::Elevation elev_;
    // clang-format on
};

} // namespace graphs
