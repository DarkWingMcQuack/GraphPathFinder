#pragma once

#include <array>
#include <fmt/core.h>
#include <string_view>

namespace common {
inline namespace impl {

class StringViewTokenizeIterator
{
public:
    using value_type = std::string_view;
    using difference_type = std::size_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    constexpr StringViewTokenizeIterator(std::string_view s,
                                         std::string_view delimiter,
                                         std::size_t start = 0) noexcept
        : s_(s),
          delimiter_(delimiter),
          start_(start),
          stop_(start)
    {
        while(start_ != std::string_view::npos and s_.substr(start_).starts_with(delimiter_)) {
            start_++;
        }

        if(start_ == s_.size()) {
            start_ = std::string_view::npos;
        }

        stop_ = s_.find(delimiter_, start_);
    }
    //copy and move constructable
    constexpr StringViewTokenizeIterator(const StringViewTokenizeIterator&) noexcept = default;
    constexpr StringViewTokenizeIterator(StringViewTokenizeIterator&&) noexcept = default;

    //copy and move assignable
    constexpr auto operator=(const StringViewTokenizeIterator&) noexcept
        -> StringViewTokenizeIterator& = default;
    constexpr auto operator=(StringViewTokenizeIterator&&) noexcept
        -> StringViewTokenizeIterator& = default;

    //comparable
    constexpr auto operator==(const StringViewTokenizeIterator&) const noexcept
        -> bool = default;
    constexpr auto operator!=(const StringViewTokenizeIterator&) const noexcept
        -> bool = default;

    //incrementable
    constexpr auto operator++() noexcept
        -> StringViewTokenizeIterator&
    {
        start_ = stop_;

        while(start_ != std::string_view::npos and s_.substr(start_).starts_with(delimiter_)) {
            start_++;
        }

        if(start_ == s_.size()) {
            start_ = std::string_view::npos;
        }

        stop_ = s_.find(delimiter_, start_);

        return *this;
    }

    constexpr auto operator++(int) noexcept
        -> StringViewTokenizeIterator&
    {
        return ++(*this);
    }

    //dereferencable
    constexpr auto operator*() noexcept
        -> std::string_view
    {

        return s_.substr(start_, stop_ - start_);
    }

    constexpr auto operator->() noexcept
        -> std::string_view
    {
        return s_.substr(start_, stop_ - start_);
    }

private:
    std::string_view s_;
    std::string_view delimiter_;
    std::size_t start_;
    std::size_t stop_ = std::string_view::npos;
};

} // namespace impl

class Tokenizer
{
public:
    constexpr Tokenizer(std::string_view data,
                        std::string_view delimiter) noexcept
        : data_(data),
          delimiter_(delimiter) {}

    constexpr auto begin() noexcept
        -> StringViewTokenizeIterator
    {
        return StringViewTokenizeIterator{data_, delimiter_};
    }

    constexpr auto end() noexcept
        -> StringViewTokenizeIterator
    {
        return StringViewTokenizeIterator{data_,
                                          delimiter_,
                                          std::string_view::npos};
    }

    constexpr auto cbegin() const noexcept
        -> StringViewTokenizeIterator
    {
        return StringViewTokenizeIterator{data_, delimiter_};
    }

    constexpr auto cend() const noexcept
        -> StringViewTokenizeIterator
    {
        return StringViewTokenizeIterator{data_,
                                          delimiter_,
                                          std::string_view::npos};
    }

private:
    std::string_view data_;
    std::string_view delimiter_;
};

template<auto N>
constexpr auto extractFirstN(std::string_view data,
                             std::string_view delimiter) noexcept
    -> std::array<std::string_view, N>
{
    std::array<std::string_view, N> result;

    Tokenizer tok(data, delimiter);
    std::size_t counter = 0;
    auto current = tok.begin();

    while(counter < N and current != tok.end()) {
        result[counter++] = *current;
        current++;
    }

    return result;
}

} // namespace common
