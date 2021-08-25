#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/nodes/IDBase.hpp>
#include <graphs/nodes/LatLngBase.hpp>
#include <graphs/nodes/LevelBase.hpp>
#include <string_view>

namespace graphs {


template<bool HasLevel>
class SimpleMapNode : public IDBase,
                      public LatLngBase,
                      public std::conditional_t<HasLevel,
                                                LevelBase,
                                                common::EmptyBase1>
{
public:
    constexpr SimpleMapNode(common::NodeID id,
                            common::Latitude lat,
                            common::Longitude lng) noexcept
        requires(!HasLevel)
        : IDBase(id),
          LatLngBase(lat, lng) {}

    constexpr SimpleMapNode(common::NodeID id,
                            common::Latitude lat,
                            common::Longitude lng,
                            common::NodeLevel lvl) noexcept
        requires HasLevel
        : IDBase(id),
          LatLngBase(lat, lng),
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
