#pragma once

#include <algorithms/distoracle/patches/Patch.hpp>
#include <common/BasicGraphTypes.hpp>
#include <concepts/DistanceOracle.hpp>
#include <concepts/Permutable.hpp>
#include <execution>
#include <numeric>
#include <utils/Permutation.hpp>

namespace algorithms::distoracle {

class PatchLookup
{
    using PatchType = std::pair<std::size_t, common::Weight>;


public:
    constexpr static inline bool is_threadsafe = true;

    PatchLookup(std::size_t number_of_nodes, std::vector<Patch> patches = {}) noexcept
        : in_patches_(number_of_nodes),
          out_patches_(number_of_nodes),
          number_of_patches_(patches.size())
    {
        static_assert(concepts::DistanceOracle<PatchLookup>,
                      "PatchLookup should fullfill the DistanceOracle concept");

        static_assert(concepts::NodesPermutable<PatchLookup>,
                      "PatchLookup should be able to permutate nodes");

        for(std::size_t i = 0; i < patches.size(); i++) {
            const auto& patch = patches[i];

            for(const auto& [src, dist] : patch.sources_) {
                const auto idx = src.get();
                out_patches_[idx].emplace_back(i, dist);
            }

            for(const auto& [src, dist] : patch.targets_) {
                const auto idx = src.get();
                in_patches_[idx].emplace_back(i, dist);
            }
        }
    }

    [[nodiscard]] auto distanceBetween(common::NodeID source, common::NodeID target) const noexcept
        -> common::Weight
    {
        const auto& in_l = in_patches_[target.get()];
        const auto& out_l = out_patches_[source.get()];

        const auto max_s_size = out_l.size();
        const auto max_t_size = in_l.size();

        auto s_idx = 0ul;
        auto t_idx = 0ul;

        while(s_idx < max_s_size and t_idx < max_t_size) {
            const auto src_hub = out_l[s_idx];
            const auto trg_hub = in_l[t_idx];

            if(src_hub.first == trg_hub.first) {
                return src_hub.second + trg_hub.second;
            }

            if(src_hub.first < trg_hub.first) {
                s_idx++;
            } else {
                t_idx++;
            }
        }

        return common::INFINITY_WEIGHT;
    }

    [[nodiscard]] auto numberOfNodes() const noexcept
        -> std::size_t
    {
        return in_patches_.size();
    }

    auto addPatch(const Patch& patch) noexcept
        -> void
    {
        for(const auto& [src, dist] : patch.sources_) {
            const auto idx = src.get();
            out_patches_[idx].emplace_back(number_of_patches_, dist);
        }

        for(const auto& [src, dist] : patch.targets_) {
            const auto idx = src.get();
            in_patches_[idx].emplace_back(number_of_patches_, dist);
        }

        number_of_patches_++;
    }

    auto applyNodePermutation(std::vector<std::size_t> perm,
                              const std::vector<std::size_t>& inv_perm) noexcept
        -> bool
    {
        // clang-format: off
        if(in_patches_.size() != perm.size()
           or out_patches_.size() != perm.size()
           or inv_perm.size() != perm.size()) {
            return false;
        }
        // clang-format: on

        // curried function expecting a permutation vector to produce
        // the function which updates the nodes in the in and out labels
        static const auto transform_f =
            [](const auto& inv_perm) {
                return [&](auto& l) {
                    std::transform(
                        std::execution::unseq,
                        std::begin(l),
                        std::end(l),
                        std::begin(l),
                        [&](const auto hub) {
                            const auto [node, dist] = hub;
                            const auto new_position = inv_perm[node.get()];
                            const auto new_node = common::NodeID{new_position};

                            return std::pair{new_node, dist};
                        });
                    std::sort(std::begin(l), std::end(l));
                };
            };

        std::for_each(std::execution::par,
                      std::begin(in_patches_),
                      std::end(in_patches_),
                      transform_f(inv_perm));

        std::for_each(std::execution::par,
                      std::begin(out_patches_),
                      std::end(out_patches_),
                      transform_f(inv_perm));

        in_patches_ = util::applyPermutation(std::move(in_patches_), perm);
        out_patches_ = util::applyPermutation(std::move(out_patches_), std::move(perm));

        return true;
    }

private:
    std::vector<std::vector<PatchType>> in_patches_;
    std::vector<std::vector<PatchType>> out_patches_;
    std::size_t number_of_patches_;
};

} // namespace algorithms::distoracle
