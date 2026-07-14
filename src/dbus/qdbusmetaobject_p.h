// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSMETAOBJECT_P_H
#define QDBUSMETAOBJECT_P_H

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

#include <QtDBus/private/qtdbusglobal_p.h>
#include <QtCore/qmetaobject.h>

#ifndef BOBUI_NO_DBUS

#ifdef interface
#  undef interface
#endif

BOBUI_BEGIN_NAMESPACE

class QDBusError;

struct QDBusMetaObjectPrivate;
struct Q_DBUS_EXPORT QDBusMetaObject: public QMetaObject
{
    bool cached;

    static QDBusMetaObject *createMetaObject(const QString &interface, const QString &xml,
                                             QHash<QString, QDBusMetaObject *> &map,
                                             QDBusError &error);
    ~QDBusMetaObject()
    {
        delete [] reinterpret_cast<const char *>(d.stringdata);
        delete [] d.data;
        delete [] reinterpret_cast<const QMetaType *>(d.metaTypes);
    }

    // methods (slots & signals):
    const int *inputTypesForMethod(int id) const;
    const int *outputTypesForMethod(int id) const;

    // properties:
    int propertyMetaType(int id) const;

private:
    QDBusMetaObject();
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif
