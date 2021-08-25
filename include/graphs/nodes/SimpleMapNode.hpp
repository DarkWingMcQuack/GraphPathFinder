#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
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
    }

    // clang-format off
    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleMapNode<HasLevel>>
	    requires (!HasLevel)
    // clang-format on
    {
    }
};

} // namespace graphs
