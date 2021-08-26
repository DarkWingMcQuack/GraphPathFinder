#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Parsing.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/edges/ShortcutBase.hpp>
#include <graphs/edges/WeightedEdge.hpp>
#include <string_view>

namespace graphs {

template<bool HasShortcuts>
class FMIEdge : public WeightedEdge,
                public std::conditional_t<HasShortcuts,
                                          ShortcutBase,
                                          common::EmptyBase1>
{
public:
    constexpr FMIEdge(common::NodeID src,
                      common::NodeID trg,
                      common::Weight cost,
                      common::Speed speed,
                      common::Type type) requires(!HasShortcuts)
        : WeightedEdge(src, trg, cost),
          speed_(speed),
          type_(type) {}

    constexpr FMIEdge(common::NodeID src,
                      common::NodeID trg,
                      common::Weight cost,
                      common::Speed speed,
                      common::Type type) requires HasShortcuts
        : WeightedEdge(src, trg, cost),
          ShortcutBase(),
          speed_(speed),
          type_(type) {}

    constexpr FMIEdge(common::NodeID src,
                      common::NodeID trg,
                      common::Weight cost,
                      common::Speed speed,
                      common::Type type,
                      common::EdgeID first_shortcut,
                      common::EdgeID second_shortcut) requires HasShortcuts
        : WeightedEdge(src, trg, cost),
          ShortcutBase(first_shortcut, second_shortcut),
          speed_(speed),
          type_(type) {}

    constexpr auto getSpeed() const noexcept
        -> common::Speed
    {
        return speed_;
    }

    constexpr auto getEdgeType() const noexcept
        -> common::Type
    {
        return type_;
    }

    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<FMIEdge>
    requires(!HasShortcuts)
    // clang-format on
    {
        const auto [src, trg, cost, speed, type] = common::extractFirstN<5>(str, " ");

        const auto src_opt = common::to<common::NodeID>(src);
        const auto trg_opt = common::to<common::NodeID>(trg);
        const auto cost_opt = common::to<common::Weight>(cost);
        const auto speed_opt = common::to<common::Speed>(speed);
        const auto type_opt = common::to<common::Type>(type);

        if(!src_opt or !trg_opt or !cost_opt or !speed_opt or !type_opt) {
            return std::nullopt;
        }

        return FMIEdge<HasShortcuts>{src_opt.value(),
                                     trg_opt.value(),
                                     cost_opt.value(),
                                     speed_opt.value(),
                                     type_opt.value()};
    }

    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<FMIEdge>
    requires HasShortcuts
    // clang-format on
    {
        const auto [src, trg, cost, speed, type, first_sc, second_sc] = common::extractFirstN<7>(str, " ");

        // clang-format off
        const auto src_opt       = common::to<common::NodeID>(src);
        const auto trg_opt       = common::to<common::NodeID>(trg);
        const auto cost_opt      = common::to<common::Weight>(cost);
        const auto speed_opt     = common::to<common::Speed>(speed);
        const auto type_opt      = common::to<common::Type>(type);
        const auto first_sc_opt  = common::to<common::EdgeID>(first_sc);
        const auto second_sc_opt = common::to<common::EdgeID>(second_sc);
        // clang-format on

        if(!src_opt or !trg_opt or !cost_opt or !speed_opt or !type_opt) {
            return std::nullopt;
        }

        if(!first_sc_opt or !second_sc_opt) {
            return FMIEdge<HasShortcuts>{src_opt.value(),
                                         trg_opt.value(),
                                         cost_opt.value(),
                                         speed_opt.value(),
                                         type_opt.value()};
        }

        return FMIEdge<HasShortcuts>{src_opt.value(),
                                     trg_opt.value(),
                                     cost_opt.value(),
                                     speed_opt.value(),
                                     type_opt.value(),
                                     first_sc_opt.value(),
                                     second_sc_opt.value()};
    }


private : common::Speed speed_;
    common::Type type_;
};

} // namespace graphs
