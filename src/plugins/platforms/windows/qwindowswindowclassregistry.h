// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QWINDOWSWINDOWCLASSREGISTRY_H
#define QWINDOWSWINDOWCLASSREGISTRY_H

#include "qtwindowsglobal.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qset.h>
#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaWindowClass)

class QWindow;

class QWindowsWindowClassRegistry
{
    Q_DISABLE_COPY_MOVE(QWindowsWindowClassRegistry)
public:
    QWindowsWindowClassRegistry(WNDPROC proc);
    ~QWindowsWindowClassRegistry();

    static QWindowsWindowClassRegistry *instance();

    static QString classNamePrefix();

    QString registerWindowClass(const QWindow *w);
    QString registerWindowClass(QString cname, WNDPROC proc, unsigned style = 0, HBRUSH brush = nullptr, bool icon = false);

private:
    void unregisterWindowClasses();

    static QWindowsWindowClassRegistry *m_instance;

    WNDPROC m_proc;
    QSet<QString> m_registeredWindowClassNames;
};

QT_END_NAMESPACE

#endif // QWINDOWSWINDOWCLASSREGISTRY_H
