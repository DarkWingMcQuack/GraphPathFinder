#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Parsing.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/common/IDBase.hpp>
#include <graphs/common/LevelBase.hpp>
#include <graphs/nodes/LatLngBase.hpp>
#include <string_view>

namespace graphs {


template<bool HasLevel>
class SimpleMapNode : public LatLngBase,
                      public std::conditional_t<HasLevel,
                                                LevelBase,
                                                common::EmptyBase1>
{
public:
    constexpr SimpleMapNode(common::Latitude lat,
                            common::Longitude lng) noexcept
        requires(!HasLevel)
        : LatLngBase(lat, lng) {}

    constexpr SimpleMapNode(common::Latitude lat,
                            common::Longitude lng,
                            common::NodeLevel lvl) noexcept
        requires HasLevel
        : LatLngBase(lat, lng),
          LevelBase(lvl) {}

    // clang-format off
    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleMapNode<HasLevel>>
	    requires HasLevel
    // clang-format on
    {
        auto [_, lat_sv, lng_sv, lvl_sv] = common::extractFirstN<4>(str, ",");
        auto lat_opt = common::to<common::Latitude>(lat_sv);
        auto lng_opt = common::to<common::Longitude>(lng_sv);
        auto lvl_opt = common::to<common::NodeLevel>(lng_sv);

        if(!lat_opt or !lng_opt or !lvl_opt) {
            return std::nullopt;
        }

        return SimpleMapNode<HasLevel>{lat_opt.value(),
                                       lng_opt.value(),
                                       lvl_opt.value()};
    }

    // clang-format off
    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleMapNode<HasLevel>>
	    requires (!HasLevel)
    // clang-format on
    {
        auto [_, lat_sv, lng_sv] = common::extractFirstN<3>(str, ",");

        auto lat_opt = common::to<common::Latitude>(lat_sv);
        auto lng_opt = common::to<common::Longitude>(lng_sv);

        if(!lat_opt or !lng_opt) {
            return std::nullopt;
        }

        return SimpleMapNode<HasLevel>{lat_opt.value(),
                                       lng_opt.value()};
    }
};

} // namespace graphs
