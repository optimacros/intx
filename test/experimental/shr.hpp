// intx: extended precision integer library.
// Copyright 2019-2020 Pawel Bylica.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <intx/intx.hpp>

namespace intx
{
namespace experimental
{
template <unsigned N>
inline auto shrd(typename uint<N>::word_type x1, typename uint<N>::word_type x2, uint64_t c)
{
    return (x2 >> c) | (x1 << (uint<N>::word_num_bits - c));
}

template <unsigned N>
uint<N> shr_c(const uint<N>& x, const uint64_t& shift) noexcept
{
    uint<2 * N> extended;
    for (unsigned i = 0; i < uint<N>::num_words; ++i)
        extended[i] = x[i];

    const auto sw =
        shift >= uint<N>::num_bits ? uint<N>::num_words : shift / uint<N>::word_num_bits;

    uint<N> r;
    for (unsigned i = 0; i < uint<N>::num_words; ++i)
        r[i] = extended[size_t(sw + i)];

    const auto sb = shift % uint<N>::word_num_bits;
    if (sb == 0)
        return r;

    constexpr auto nw = uint<N>::num_words;

    uint<N> z;
    z[nw - 1] = r[nw - 1] >> sb;

    for (unsigned i = 0; i < nw - 1; ++i)
        z[nw - i - 2] = shrd<N>(r[nw - i - 1], r[nw - i - 2], sb);

    return z;
}

template <unsigned N>
inline constexpr uint<N> shr_c(const uint<N>& x, const uint<N>& shift) noexcept
{
    uint64_t high_words_fold = 0;
    for (size_t i = 1; i < uint<N>::num_words; ++i)
        high_words_fold |= shift[i];

    if (INTX_UNLIKELY(high_words_fold != 0))
        return 0;

    return shr_c(x, shift[0]);
}

}  // namespace experimental
}  // namespace intx