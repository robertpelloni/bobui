// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSINTERFACE_H
#define QDBUSINTERFACE_H

#include <QtDBus/qtdbusglobal.h>
#include <QtDBus/qdbusabstractinterface.h>
#include <QtDBus/qdbusconnection.h>

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE


class QDBusInterfacePrivate;
class Q_DBUS_EXPORT QDBusInterface: public QDBusAbstractInterface
{
    friend class QDBusConnection;
private:
    QDBusInterface(QDBusInterfacePrivate *p);

public:
    QDBusInterface(const QString &service, const QString &path, const QString &interface = QString(),
                   const QDBusConnection &connection = QDBusConnection::sessionBus(),
                   QObject *parent = nullptr);
    ~QDBusInterface();

    virtual const QMetaObject *metaObject() const override;
    virtual void *bobui_metacast(const char *) override;
    virtual int bobui_metacall(QMetaObject::Call, int, void **) override;

private:
    Q_DECLARE_PRIVATE(QDBusInterface)
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif
