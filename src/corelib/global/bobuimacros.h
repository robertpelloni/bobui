// Copyright (C) 2026 The BobUI Project
// SPDX-License-Identifier: BSD-3-Clause

#ifndef BOBUIMACROS_H
#define BOBUIMACROS_H

#include <QtCore/qglobal.h>

#define BOBUI_BEGIN_NAMESPACE QT_BEGIN_NAMESPACE
#define BOBUI_END_NAMESPACE QT_END_NAMESPACE
#define BOBUI_USE_NAMESPACE QT_USE_NAMESPACE

#ifdef __cplusplus


// Core Class Aliases
namespace BobUI {
    using Object = ::QObject;
    using String = ::QString;
    using Variant = ::QVariant;
}
#endif

#endif // BOBUIMACROS_H
