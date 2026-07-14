// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#define BOBUI_XML_BUILD_REMOVED_API

#include "qtxmlglobal.h"

BOBUI_USE_NAMESPACE

#if BOBUI_XML_REMOVED_SINCE(6, 9)

#if BOBUI_CONFIG(dom)

#include "qdom.h"

bool QDomNodeList::operator==(const QDomNodeList &other) const
{
    return comparesEqual(*this, other);
}

bool QDomNodeList::operator!=(const QDomNodeList &other) const
{
    return !comparesEqual(*this, other);
}

#endif // BOBUI_CONFIG(dom)

// #include <qotherheader.h>
// // implement removed functions from qotherheader.h
// order sections alphabetically to reduce chances of merge conflicts

#endif // BOBUI_XML_REMOVED_SINCE(6, 9)
