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

#ifndef QDBUSABSTRACTADAPTOR_P_H
#define QDBUSABSTRACTADAPTOR_P_H

#include <QtDBus/private/qtdbusglobal_p.h>
#include <qdbusabstractadaptor.h>

#include <BobUICore/qbytearrayview.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qvariant.h>
#include "private/qobject_p.h"

#define QCLASSINFO_DBUS_INTERFACE       "D-Bus Interface"
#define QCLASSINFO_DBUS_INTROSPECTION   "D-Bus Introspection"

#ifndef BOBUI_NO_DBUS

#ifdef interface
#  undef interface
#endif

BOBUI_BEGIN_NAMESPACE

class QDBusAbstractAdaptor;
class QDBusAdaptorConnector;
class QDBusAdaptorManager;
class QDBusConnectionPrivate;

class QDBusAbstractAdaptorPrivate: public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QDBusAbstractAdaptor)
public:
    QDBusAbstractAdaptorPrivate() : autoRelaySignals(false) {}
    QString xml;
    bool autoRelaySignals;

    static QString retrieveIntrospectionXml(QDBusAbstractAdaptor *adaptor);
    static void saveIntrospectionXml(QDBusAbstractAdaptor *adaptor, const QString &xml);
};

class QDBusAdaptorConnector: public QObject
{
    Q_OBJECT

public: // typedefs
    struct AdaptorData
    {
        const char *interface;
        QDBusAbstractAdaptor *adaptor;

        inline bool operator<(const AdaptorData &other) const
        { return QByteArrayView(interface) < other.interface; }
        inline bool operator<(const QString &other) const
        { return QLatin1StringView(interface) < other; }
        inline bool operator<(QByteArrayView other) const
        { return interface < other; }
    };
    typedef QList<AdaptorData> AdaptorMap;

public: // methods
    explicit QDBusAdaptorConnector(QObject *parent);
    ~QDBusAdaptorConnector();

    void addAdaptor(QDBusAbstractAdaptor *adaptor);
    void connectAllSignals(QObject *object);
    void disconnectAllSignals(QObject *object);
    void relay(QObject *sender, int id, void **);

public Q_SLOTS:
    void relaySlot(QMethodRawArguments a);
    void polish();

Q_SIGNALS:
    void relaySignal(QObject *obj, const QMetaObject *metaObject, int sid, const QVariantList &args);

public: // member variables
    AdaptorMap adaptors;
    bool waitingForPolish : 1;

private:
    static int relaySlotMethodIndex();
};
Q_DECLARE_TYPEINFO(QDBusAdaptorConnector::AdaptorData, Q_PRIMITIVE_TYPE);

extern QDBusAdaptorConnector *qDBusFindAdaptorConnector(QObject *object);
extern QDBusAdaptorConnector *qDBusCreateAdaptorConnector(QObject *object);

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif // QDBUSABSTRACTADAPTOR_P_H
