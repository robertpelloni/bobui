// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSVIRTUALOBJECT_H
#define QDBUSVIRTUALOBJECT_H

#include <QtDBus/qtdbusglobal.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE


class QDBusMessage;
class QDBusConnection;

class Q_DBUS_EXPORT QDBusVirtualObject : public QObject
{
    Q_OBJECT
public:
    explicit QDBusVirtualObject(QObject *parent = nullptr);
    virtual ~QDBusVirtualObject();

    virtual QString introspect(const QString &path) const = 0;
    virtual bool handleMessage(const QDBusMessage &message, const QDBusConnection &connection) = 0;

private:
    Q_DISABLE_COPY(QDBusVirtualObject)
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif
