#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <graphs/nodes/IDBase.hpp>
#include <graphs/nodes/LatLngBase.hpp>
#include <graphs/nodes/LevelBase.hpp>

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
                            double lat,
                            double lng) noexcept
        requires(!HasLevel)
        : IDBase(id),
          LatLngBase(lat, lng) {}

    constexpr SimpleMapNode(common::NodeID id,
                            double lat,
                            double lng,
                            common::NodeLevel lvl) noexcept
        requires HasLevel
        : IDBase(id),
          LatLngBase(lat, lng),
          LevelBase(lvl) {}

    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleMapNode<HasLevel>>
    {
    }
};

} // namespace graphs
