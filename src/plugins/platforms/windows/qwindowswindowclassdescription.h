// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSWINDOWCLASSDESCRIPTION_H
#define QWINDOWSWINDOWCLASSDESCRIPTION_H

#include "qtwindowsglobal.h"

#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

class QWindow;

struct QWindowsWindowClassDescription
{
    static QWindowsWindowClassDescription fromName(QString name, WNDPROC procedure);
    static QWindowsWindowClassDescription fromWindow(const QWindow *window, WNDPROC procedure);

    QString name;
    WNDPROC procedure{ DefWindowProc };
    unsigned int style{ 0 };
    HBRUSH brush{ nullptr };
    bool hasIcon{ false };
    bool shouldAddPrefix{ true };

private:
    friend QDebug operator<<(QDebug dbg, const QWindowsWindowClassDescription &description);
};

QT_END_NAMESPACE

#endif // QWINDOWSWINDOWCLASSDESCRIPTION_H
