// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSXMLPARSER_P_H
#define QDBUSXMLPARSER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtDBus/private/qtdbusglobal_p.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qmap.h>
#include <QtCore/qxmlstream.h>
#include "qdbusintrospection_p.h"

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(dbusParser)

/*!
    \internal
*/
class QDBusXmlParser
{
    QString m_service;
    QString m_path;
    QSharedDataPointer<QDBusIntrospection::Object> m_object;
    std::unique_ptr<QDBusIntrospection::Interface> m_currentInterface;
    QDBusIntrospection::Interfaces m_interfaces;
    QXmlStreamReader m_xml;
    QDBusIntrospection::SourceLocation m_currentLocation;
    QDBusIntrospection::DiagnosticsReporter *m_reporter;

public:
    QDBusXmlParser(const QString &service, const QString &path, const QString &xmlData,
                   QDBusIntrospection::DiagnosticsReporter *reporter = nullptr);

    inline QDBusIntrospection::Interfaces interfaces() const { return m_interfaces; }
    inline QSharedDataPointer<QDBusIntrospection::Object> object() const { return m_object; }

private:
    void readNode(int nodeLevel);
    void readInterface();
    bool parseSignal(QDBusIntrospection::Signal &signalData);
    bool parseMethod(QDBusIntrospection::Method &methodData);
    bool parseProperty(QDBusIntrospection::Property &propertyData);
    bool parseAnnotation(QDBusIntrospection::Annotations &annotations,
                         bool interfaceAnnotation = false);
    bool parseArg(const QXmlStreamAttributes &attributes, QDBusIntrospection::Argument &argData);
    bool readNextStartElement();
    void updateCurrentLocation();
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
#endif
