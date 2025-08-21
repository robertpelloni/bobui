// Copyright (C) 2025 Intel Corporation.
// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef Q20BIT_H
#define Q20BIT_H

#include <QtCore/q20type_traits.h>

#ifdef __cpp_lib_bitops
#  include <bit>
#else

#  ifdef Q_CC_MSVC
// avoiding qsimd.h -> immintrin.h unless necessary, because it increases
// compilation time
#    include <QtCore/qsimd.h>
#    include <intrin.h>
#  endif
#endif

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. Types and functions defined in this
// file can reliably be replaced by their std counterparts, once available.
// You may use these definitions in your own code, but be aware that we
// will remove them once Qt depends on the C++ version that supports
// them in namespace std. There will be NO deprecation warning, the
// definitions will JUST go away.
//
// If you can't agree to these terms, don't use these definitions!
//
// We mean it.
//

QT_BEGIN_NAMESPACE

namespace q20 {
#if defined(__cpp_lib_bitops)
using std::popcount;
#else
namespace detail {
template <typename T> /*non-constexpr*/ inline auto hw_popcount(T v) noexcept
{
#if defined(Q_CC_MSVC) && defined(Q_PROCESSOR_X86) && defined(__POPCNT__)
    // Note: __POPCNT__ comes from qsimd.h, not the compiler.
#  ifdef Q_PROCESSOR_X86_64
    if constexpr (sizeof(T) == sizeof(quint64))
        return int(__popcnt64(v));
#  endif
    if constexpr (sizeof(T) == sizeof(quint64))
        return int(__popcnt(quint32(v)) + __popcnt(quint32(v >> 32)));
    if constexpr (sizeof(T) == sizeof(quint32))
        return int(__popcnt(v));
    return int(__popcnt16(v));
#else
    Q_UNUSED(v);
#endif
}
} // namespace q20::detail

template <typename T> constexpr std::enable_if_t<std::is_unsigned_v<T>, int>
popcount(T v) noexcept
{
#  if __has_builtin(__builtin_popcount)
    // These GCC/Clang intrinsics are constexpr and use the HW instructions
    // where available. Note: no runtime detection.
    if constexpr (sizeof(T) > sizeof(quint32))
        return __builtin_popcountll(v);
    return __builtin_popcount(v);
#  endif

#  ifdef QT_SUPPORTS_IS_CONSTANT_EVALUATED
    // Try hardware functions if not constexpr. Note: no runtime detection.
    if (!is_constant_evaluated()) {
        if constexpr (std::is_integral_v<decltype(detail::hw_popcount(v))>)
            return detail::hw_popcount(v);
    }
#  endif

    constexpr int Digits = std::numeric_limits<T>::digits;
    int r =  (((v      ) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f;
    if constexpr (Digits > 12)
        r += (((v >> 12) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f;
    if constexpr (Digits > 24)
        r += (((v >> 24) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f;
    if constexpr (Digits > 36) {
        r += (((v >> 36) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f +
             (((v >> 48) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f +
             (((v >> 60) & 0xfff)    * Q_UINT64_C(0x1001001001001) & Q_UINT64_C(0x84210842108421)) % 0x1f;
    }
    return r;
}
#endif // __cpp_lib_bitops
} // namespace q20

QT_END_NAMESPACE

#endif // Q20BIT_H
