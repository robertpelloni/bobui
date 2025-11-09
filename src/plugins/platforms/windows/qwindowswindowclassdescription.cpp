// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowswindowclassdescription.h"

#include <QtGui/qwindow.h>

#include "qwindowswindowclassregistry.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

QWindowsWindowClassDescription QWindowsWindowClassDescription::fromName(QString name, WNDPROC procedure)
{
    return { std::move(name), procedure };
}

QWindowsWindowClassDescription QWindowsWindowClassDescription::fromWindow(const QWindow *window, WNDPROC procedure)
{
    Q_ASSERT(window);

    QWindowsWindowClassDescription description;
    description.procedure = procedure;

    const Qt::WindowFlags flags = window->flags();
    const Qt::WindowFlags type = flags & Qt::WindowType_Mask;
    // Determine style and icon.
    description.style = CS_DBLCLKS;
    description.hasIcon = true;
    // The following will not set CS_OWNDC for any widget window, even if it contains a
    // QOpenGLWidget or QQuickWidget later on. That cannot be detected at this stage.
    if (window->surfaceType() == QSurface::OpenGLSurface || (flags & Qt::MSWindowsOwnDC))
        description.style |= CS_OWNDC;
    if (!(flags & Qt::NoDropShadowWindowHint)
        && (type == Qt::Popup || window->property("_q_windowsDropShadow").toBool())) {
        description.style |= CS_DROPSHADOW;
    }
    switch (type) {
        case Qt::Tool:
        case Qt::ToolTip:
        case Qt::Popup:
            description.style |= CS_SAVEBITS; // Save/restore background
            description.hasIcon = false;
            break;
        case Qt::Dialog:
            if (!(flags & Qt::WindowSystemMenuHint))
                description.hasIcon = false; // QTBUG-2027, dialogs without system menu.
            break;
    }
    // Create a unique name for the flag combination
    description.name = "QWindow"_L1;
    switch (type) {
        case Qt::Tool:
            description.name += "Tool"_L1;
            break;
        case Qt::ToolTip:
            description.name += "ToolTip"_L1;
            break;
        case Qt::Popup:
            description.name += "Popup"_L1;
            break;
        default:
            break;
    }
    if (description.style & CS_DROPSHADOW)
        description.name += "DropShadow"_L1;
    if (description.style & CS_SAVEBITS)
        description.name += "SaveBits"_L1;
    if (description.style & CS_OWNDC)
        description.name += "OwnDC"_L1;
    if (description.hasIcon)
        description.name += "Icon"_L1;

    return description;
}

QT_END_NAMESPACE
