// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSABSTRACTADAPTOR_H
#define QDBUSABSTRACTADAPTOR_H

#include <QtDBus/qtdbusglobal.h>
#include <QtCore/qobject.h>

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE


class QDBusAbstractAdaptorPrivate;
class Q_DBUS_EXPORT QDBusAbstractAdaptor: public QObject
{
    Q_OBJECT
protected:
    explicit QDBusAbstractAdaptor(QObject *parent);

public:
    ~QDBusAbstractAdaptor();

protected:
    void setAutoRelaySignals(bool enable);
    bool autoRelaySignals() const;

private:
    Q_DECLARE_PRIVATE(QDBusAbstractAdaptor)
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS

#endif
