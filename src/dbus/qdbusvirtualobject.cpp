// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdbusvirtualobject.h"

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE

/*!
    Constructs a QDBusVirtualObject with \a parent.
*/
QDBusVirtualObject::QDBusVirtualObject(QObject *parent) :
    QObject(parent)
{
}

/*!
    Destroys the object, deleting all of its child objects.
*/
QDBusVirtualObject::~QDBusVirtualObject()
{
}

BOBUI_END_NAMESPACE

#include "moc_qdbusvirtualobject.cpp"

/*!
    \class QDBusVirtualObject
    \inmodule BobUIDBus
    \since 5.1

    \brief The QDBusVirtualObject class is used to handle several DBus paths with one class.
*/

/*!
    \fn bool QDBusVirtualObject::handleMessage(const QDBusMessage &message, const QDBusConnection &connection) = 0

    This function needs to handle all messages to the path of the
    virtual object, when the SubPath option is specified.
    The service, path, interface and methods are all part of the \a message.
    Parameter \a connection is the connection handle.
    Must return \c true when the message is handled, otherwise \c false (will generate dbus error message).
*/


/*!
    \fn QString QDBusVirtualObject::introspect(const QString &path) const

    This function needs to handle the introspection of the
    virtual object on \a path. It must return xml of the form:

    \code
<interface name="org.qtproject.BobUIDBus.MyObject" >
    <property access="readwrite" type="i" name="prop1" />
</interface>
    \endcode

    If you pass the SubPath option, this introspection has to include all child nodes.
    Otherwise QDBus handles the introspection of the child nodes.
*/

#endif // BOBUI_NO_DBUS
