#pragma once

#include <functional>
#include <utility>
#include <vector>

namespace util {


[[nodiscard]] inline auto isValidPermutation(const std::vector<std::size_t>& permutation) noexcept
    -> bool
{
    std::vector b(permutation.size(), false);
    for(auto i : permutation) {
        b[i] = true;
    }

    return std::all_of(std::begin(b),
                       std::end(b),
                       [](auto b) { return b; });
}

template<class T>
[[nodiscard]] auto applyPermutation(std::vector<T> vec,
                                    std::vector<std::size_t> permutation) noexcept
    -> std::vector<T>
{
    for(std::size_t i = 0; i < vec.size(); i++) {
        std::size_t curr = i;
        std::size_t next = permutation[curr];
        while(next != i) {
            std::swap(vec[curr], vec[next]);
            permutation[curr] = curr;
            curr = next;
            next = permutation[next];
        }
        permutation[curr] = curr;
    }

    return std::move(vec);
}


// permutations are a monoid, we can combine them
[[nodiscard]] inline auto combine(const std::vector<std::size_t>& first,
                                  const std::vector<std::size_t>& second) noexcept
    -> std::vector<std::size_t>
{
    std::vector<std::size_t> combined(first.size());

    for(std::size_t i = 0; i < first.size(); i++) {
        combined[i] = second[first[i]];
    }

    return combined;
}

/**
  * given a permutation vector this method returns an the inverse permutation vector such that for all i:
  * i == inverse[permutate[i]] and i == permutate[inverse[i]]
  */
[[nodiscard]] inline auto inversePermutation(const std::vector<std::size_t>& permutation) noexcept
    -> std::vector<std::size_t>
{
    std::vector<std::size_t> inverse_perm(permutation.size(), 0);
    for(std::size_t i = 0; i < permutation.size(); i++) {
        inverse_perm[permutation[i]] = i;
    }

    return inverse_perm;
}

} // namespace util
