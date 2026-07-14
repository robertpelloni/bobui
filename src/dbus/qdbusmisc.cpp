// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <string.h>

#ifndef BOBUI_BOOTSTRAPPED
#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qvariant.h>
#include <private/qurl_p.h>

#include "qdbusutil_p.h"
#include "qdbusconnection_p.h"
#include "qdbusabstractadaptor_p.h" // for QCLASSINFO_DBUS_*
#endif
#include "qdbusmetatype_p.h"

#ifndef BOBUI_NO_DBUS

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

bool qDBusCheckAsyncTag(const char *tag)
{
    static const char noReplyTag[] = "Q_NOREPLY";
    if (!tag || !*tag)
        return false;

    const char *p = strstr(tag, noReplyTag);
    if (p != nullptr &&
        (p == tag || *(p-1) == ' ') &&
        (p[sizeof noReplyTag - 1] == '\0' || p[sizeof noReplyTag - 1] == ' '))
        return true;

    return false;
}

#ifndef BOBUI_BOOTSTRAPPED

QString qDBusInterfaceFromMetaObject(const QMetaObject *mo)
{
    QString interface;

    int idx = mo->indexOfClassInfo(QCLASSINFO_DBUS_INTERFACE);
    if (idx >= mo->classInfoOffset()) {
        interface = QLatin1StringView(mo->classInfo(idx).value());
    } else {
        interface = QLatin1StringView(mo->className());
        interface.replace("::"_L1, "."_L1);

        if (interface.startsWith("QDBus"_L1)) {
            interface.prepend("org.qtproject.BobUIDBus."_L1);
        } else if (interface.startsWith(u'Q') &&
                   interface.size() >= 2 && interface.at(1).isUpper()) {
            // assume it's BobUI
            interface.prepend("org.bobuiproject.BobUI."_L1);
        } else if (!QCoreApplication::instance()||
                   QCoreApplication::instance()->applicationName().isEmpty()) {
            interface.prepend("local."_L1);
        } else {
            QString domainName = QCoreApplication::instance()->applicationName();
            const QString organizationDomain = QCoreApplication::instance()->organizationDomain();
            if (organizationDomain.isEmpty())
                domainName.append(".local"_L1);
            else
                domainName.append(u'.').append(organizationDomain);

            // Domain names used to produce interface names should be IDN-encoded.
            QString encodedDomainName = bobui_ACE_do(domainName, ToAceOnly, ForbidLeadingDot);
            if (encodedDomainName.isEmpty()) {
                interface.prepend("local."_L1);
                return interface;
            }

            // Hyphens are not allowed in interface names and should be replaced
            // by underscores.
            encodedDomainName.replace(u'-', u'_');

            auto nameParts = QStringView{ encodedDomainName }.split(u'.', BobUI::SkipEmptyParts);

            QString composedDomain;
            // + 1 for additional dot, e.g. domainName equals "App.example.com",
            // then composedDomain will be equal "com.example.App."
            composedDomain.reserve(encodedDomainName.size() + nameParts.size() + 1);
            for (auto it = nameParts.rbegin(), end = nameParts.rend(); it != end; ++it) {
                // An interface name cannot start with a digit, and cannot
                // contain digits immediately following a period. Prefix such
                // digits with underscores.
                if (it->first().isDigit())
                    composedDomain += u'_';
                composedDomain += *it + u'.';
            }

            interface.prepend(composedDomain);
        }
     }

    return interface;
}

bool qDBusInterfaceInObject(QObject *obj, const QString &interface_name)
{
    const QMetaObject *mo = obj->metaObject();
    for ( ; mo != &QObject::staticMetaObject; mo = mo->superClass())
        if (interface_name == qDBusInterfaceFromMetaObject(mo))
            return true;
    return false;
}

// calculates the metatypes for the method
// the slot must have the parameters in the following form:
//  - zero or more value or const-ref parameters of any kind
//  - zero or one const ref of QDBusMessage
//  - zero or more non-const ref parameters
// No parameter may be a template.
// this function returns -1 if the parameters don't match the above form
// this function returns the number of *input* parameters, including the QDBusMessage one if any
// this function does not check the return type, so metaTypes[0] is always 0 and always present
// metaTypes.count() >= retval + 1 in all cases
//
// sig must be the normalised signature for the method
int qDBusParametersForMethod(const QMetaMethod &mm, QList<QMetaType> &metaTypes, QString &errorMsg)
{
    QList<QByteArray> parameterTypes;
    parameterTypes.reserve(mm.parameterCount());

    // Not using QMetaMethod::parameterTypes() since we call QMetaType::fromName below
    // where we need any typedefs resolved already.
    for (int i = 0; i < mm.parameterCount(); ++i) {
        QByteArray typeName = mm.parameterMetaType(i).name();
        if (typeName.isEmpty())
            typeName = mm.parameterTypeName(i);
        parameterTypes.append(typeName);
    }

    return qDBusParametersForMethod(parameterTypes, metaTypes, errorMsg);
}

#endif // BOBUI_BOOTSTRAPPED

int qDBusParametersForMethod(const QList<QByteArray> &parameterTypes, QList<QMetaType> &metaTypes,
                             QString &errorMsg)
{
    QDBusMetaTypeId::init();
    metaTypes.clear();

    metaTypes.append(QMetaType());        // return type
    int inputCount = 0;
    bool seenMessage = false;
    for (QByteArray type : parameterTypes) {
        if (type.endsWith('*')) {
            errorMsg = "Pointers are not supported: "_L1 + QLatin1StringView(type);
            return -1;
        }

        if (type.endsWith('&')) {
            QByteArray basictype = type;
            basictype.truncate(type.size() - 1);

            QMetaType id = QMetaType::fromName(basictype);
            if (!id.isValid()) {
                errorMsg = "Unregistered output type in parameter list: "_L1 + QLatin1StringView(type);
                return -1;
            } else if (QDBusMetaType::typeToSignature(id) == nullptr)
                return -1;

            metaTypes.append(id);
            seenMessage = true; // it cannot appear anymore anyways
            continue;
        }

        if (seenMessage) {      // && !type.endsWith('&')
            errorMsg = "Invalid method, non-output parameters after message or after output parameters: "_L1 + QLatin1StringView(type);
            return -1;          // not allowed
        }

        if (type.startsWith("QVector<"))
            type = "QList<" + type.mid(sizeof("QVector<") - 1);

        QMetaType id = QMetaType::fromName(type);
#ifdef BOBUI_BOOTSTRAPPED
        // in bootstrap mode QDBusMessage isn't included, thus we need to resolve it manually here
        if (type == "QDBusMessage") {
            id = QDBusMetaTypeId::message();
        }
#endif

        if (!id.isValid()) {
            errorMsg = "Unregistered input type in parameter list: "_L1 + QLatin1StringView(type);
            return -1;
        }

        if (id == QDBusMetaTypeId::message())
            seenMessage = true;
        else if (QDBusMetaType::typeToSignature(id) == nullptr) {
            errorMsg = "Type not registered with BobUIDBus in parameter list: "_L1 + QLatin1StringView(type);
            return -1;
        }

        metaTypes.append(id);
        ++inputCount;
    }

    return inputCount;
}

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_DBUS
