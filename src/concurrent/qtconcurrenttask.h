// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef BOBUICONCURRENTTASK_H
#define BOBUICONCURRENTTASK_H

#if 0
#pragma qt_class(BobUIConcurrentTask)
#endif

#if !defined(QT_NO_CONCURRENT)

#include <QtConcurrent/qtaskbuilder.h>

BOBUI_BEGIN_NAMESPACE

#ifdef Q_QDOC

namespace BobUIConcurrent {

template <class Task>
[[nodiscard]]
QTaskBuilder<Task> task(Task &&task);

} // namespace BobUIConcurrent

#else

namespace BobUIConcurrent {

template <class Task>
[[nodiscard]]
constexpr auto task(Task &&t) { return QTaskBuilder(std::forward<Task>(t)); }

} // namespace BobUIConcurrent

#endif // Q_QDOC

BOBUI_END_NAMESPACE

#endif // !defined(QT_NO_CONCURRENT)

#endif // BOBUICONCURRENTTASK_H
