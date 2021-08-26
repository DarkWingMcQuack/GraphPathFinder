#pragma once

#include <common/BasicGraphTypes.hpp>
#include <common/EmptyBase.hpp>
#include <common/Parsing.hpp>
#include <common/Tokenizer.hpp>
#include <graphs/edges/SrcBase.hpp>
#include <graphs/edges/TrgBase.hpp>
#include <string_view>

namespace graphs {

class SimpleEdge : public SrcBase,
                   public TrgBase
{
public:
    constexpr SimpleEdge(common::NodeID src,
                         common::NodeID trg)
        : SrcBase(src),
          TrgBase(trg) {}

    static auto parse(std::string_view str) noexcept
        -> std::optional<SimpleEdge>
    // clang-format on
    {
        const auto [src, trg] = common::extractFirstN<2>(str, ",");

        const auto src_opt = common::to<common::NodeID>(src);
        const auto trg_opt = common::to<common::NodeID>(trg);

        if(!src_opt or !trg_opt) {
            return std::nullopt;
        }

        return SimpleEdge{src_opt.value(),
                          trg_opt.value()};
    }
};

} // namespace graphs
