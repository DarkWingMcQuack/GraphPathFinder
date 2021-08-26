#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Parsing.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/common/WeightBase.hpp>
#include <graphs/edges/SimpleEdge.hpp>
#include <string_view>

namespace graphs {

class WeightedEdge : public SimpleEdge,
                     public WeightBase
{

public:
    constexpr WeightedEdge(common::NodeID src,
                           common::NodeID trg,
                           common::Weight weight)
        : SimpleEdge(src, trg),
          WeightBase(weight) {}

    constexpr static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleEdge>
    // clang-format on
    {
        const auto [src, trg, weight] = common::extractFirstN<3>(str, ",");

        const auto src_opt = common::to<common::NodeID>(src);
        const auto trg_opt = common::to<common::NodeID>(trg);
        const auto weight_opt = common::to<common::Weight>(weight);

        if(!src_opt or !trg_opt or !weight_opt) {
            return std::nullopt;
        }

        return WeightedEdge{src_opt.value(),
                            trg_opt.value(),
                            weight_opt.value()};
    }
};

} // namespace graphs
