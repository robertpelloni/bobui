// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICONCURRENT_COMPILERTEST_H
#define BOBUICONCURRENT_COMPILERTEST_H

#include <QtConcurrent/qtconcurrent_global.h>

#ifndef BOBUI_NO_CONCURRENT

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {

    template <class T, typename = void>
    struct IsIterable : std::false_type {};
    template <class T>
    struct IsIterable<T, std::void_t<decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end())>>
        : std::true_type
    { };

    template <class T>
    inline constexpr bool IsIterableValue = IsIterable<T>::value;

    template <class T, typename = void>
    struct IsDereferenceable : std::false_type {};
    template <class T>
    struct IsDereferenceable<T, std::void_t<decltype(*std::declval<T>())>>
        : std::true_type
    { };

    template <class T>
    inline constexpr bool IsDereferenceableValue = IsDereferenceable<T>::value;
}

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CONCURRENT

#endif
