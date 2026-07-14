// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QDBUSINTERFACE_P_H
#define QDBUSINTERFACE_P_H

#include <QtDBus/private/qtdbusglobal_p.h>
#include "qdbusabstractinterface_p.h"
#include "qdbusmetaobject_p.h"
#include <qdbusinterface.h>

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE

class QDBusInterfacePrivate: public QDBusAbstractInterfacePrivate
{
public:
    Q_DECLARE_PUBLIC(QDBusInterface)

    QDBusMetaObject *metaObject;

    QDBusInterfacePrivate(const QString &serv, const QString &p, const QString &iface,
                          const QDBusConnection &con);
    ~QDBusInterfacePrivate();

    int metacall(QMetaObject::Call c, int id, void **argv);
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif
