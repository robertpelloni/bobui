// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include "qwindowswindowclassregistry.h"

#include <QtCore/qlibraryinfo.h>
#include <QtCore/quuid.h>
#include <QtGui/qwindow.h>

#include "qwindowscontext.h"

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

Q_LOGGING_CATEGORY(lcQpaWindowClass, "qt.qpa.windowclass")

QWindowsWindowClassRegistry *QWindowsWindowClassRegistry::m_instance = nullptr;

QWindowsWindowClassRegistry::QWindowsWindowClassRegistry(WNDPROC proc)
    : m_proc(proc)
{
    m_instance = this;
}

QWindowsWindowClassRegistry::~QWindowsWindowClassRegistry()
{
    unregisterWindowClasses();

    m_instance = nullptr;
}

QWindowsWindowClassRegistry *QWindowsWindowClassRegistry::instance()
{
    return m_instance;
}

QString QWindowsWindowClassRegistry::classNamePrefix()
{
    static QString result;
    if (result.isEmpty()) {
        QTextStream str(&result);
        str << "Qt" << QT_VERSION_MAJOR << QT_VERSION_MINOR << QT_VERSION_PATCH;
        if (QLibraryInfo::isDebugBuild())
            str << 'd';
#ifdef QT_NAMESPACE
#  define xstr(s) str(s)
#  define str(s) #s
        str << xstr(QT_NAMESPACE);
#  undef str
#  undef xstr
#endif
    }
    return result;
}

QString QWindowsWindowClassRegistry::registerWindowClass(const QWindow *w)
{
    Q_ASSERT(w);
    const Qt::WindowFlags flags = w->flags();
    const Qt::WindowFlags type = flags & Qt::WindowType_Mask;
    // Determine style and icon.
    uint style = CS_DBLCLKS;
    bool icon = true;
    // The following will not set CS_OWNDC for any widget window, even if it contains a
    // QOpenGLWidget or QQuickWidget later on. That cannot be detected at this stage.
    if (w->surfaceType() == QSurface::OpenGLSurface || (flags & Qt::MSWindowsOwnDC))
        style |= CS_OWNDC;
    if (!(flags & Qt::NoDropShadowWindowHint)
        && (type == Qt::Popup || w->property("_q_windowsDropShadow").toBool())) {
        style |= CS_DROPSHADOW;
    }
    switch (type) {
        case Qt::Tool:
        case Qt::ToolTip:
        case Qt::Popup:
            style |= CS_SAVEBITS; // Save/restore background
            icon = false;
            break;
        case Qt::Dialog:
            if (!(flags & Qt::WindowSystemMenuHint))
                icon = false; // QTBUG-2027, dialogs without system menu.
            break;
    }
    // Create a unique name for the flag combination
    QString cname = classNamePrefix();
    cname += "QWindow"_L1;
    switch (type) {
        case Qt::Tool:
            cname += "Tool"_L1;
            break;
        case Qt::ToolTip:
            cname += "ToolTip"_L1;
            break;
        case Qt::Popup:
            cname += "Popup"_L1;
            break;
        default:
            break;
    }
    if (style & CS_DROPSHADOW)
        cname += "DropShadow"_L1;
    if (style & CS_SAVEBITS)
        cname += "SaveBits"_L1;
    if (style & CS_OWNDC)
        cname += "OwnDC"_L1;
    if (icon)
        cname += "Icon"_L1;

    return registerWindowClass(cname, m_proc, style, nullptr, icon);
}

QString QWindowsWindowClassRegistry::registerWindowClass(QString cname, WNDPROC proc, unsigned style, HBRUSH brush, bool icon)
{
    // since multiple Qt versions can be used in one process
    // each one has to have window class names with a unique name
    // The first instance gets the unmodified name; if the class
    // has already been registered by another instance of Qt then
    // add a UUID. The check needs to be performed for each name
    // in case new message windows are added (QTBUG-81347).
    // Note: GetClassInfo() returns != 0 when a class exists.
    const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
    WNDCLASS wcinfo;
    const bool classExists = GetClassInfo(appInstance, reinterpret_cast<LPCWSTR>(cname.utf16()), &wcinfo) != FALSE
        && wcinfo.lpfnWndProc != proc;

    if (classExists)
        cname += QUuid::createUuid().toString();

    if (m_registeredWindowClassNames.contains(cname))        // already registered in our list
        return cname;

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = style;
    wc.lpfnWndProc = proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = appInstance;
    wc.hCursor = nullptr;
    wc.hbrBackground = brush;
    if (icon) {
        wc.hIcon = static_cast<HICON>(LoadImage(appInstance, L"IDI_ICON1", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
        if (wc.hIcon) {
            int sw = GetSystemMetrics(SM_CXSMICON);
            int sh = GetSystemMetrics(SM_CYSMICON);
            wc.hIconSm = static_cast<HICON>(LoadImage(appInstance, L"IDI_ICON1", IMAGE_ICON, sw, sh, 0));
        } else {
            wc.hIcon = static_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
            wc.hIconSm = nullptr;
        }
    } else {
        wc.hIcon = nullptr;
        wc.hIconSm = nullptr;
    }

    wc.lpszMenuName = nullptr;
    wc.lpszClassName = reinterpret_cast<LPCWSTR>(cname.utf16());
    ATOM atom = RegisterClassEx(&wc);
    if (!atom) {
        qErrnoWarning("QApplication::regClass: Registering window class '%s' failed.",
                      qPrintable(cname));
    }

    m_registeredWindowClassNames.insert(cname);
    qCDebug(lcQpaWindowClass).nospace() << __FUNCTION__ << ' ' << cname
        << " style=0x" << Qt::hex << style << Qt::dec
        << " brush=" << brush << " icon=" << icon << " atom=" << atom;
    return cname;
}

void QWindowsWindowClassRegistry::unregisterWindowClasses()
{
    const auto appInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

    for (const QString &name : std::as_const(m_registeredWindowClassNames)) {
        if (!UnregisterClass(reinterpret_cast<LPCWSTR>(name.utf16()), appInstance) && QWindowsContext::verbose)
            qErrnoWarning("UnregisterClass failed for '%s'", qPrintable(name));
    }
    m_registeredWindowClassNames.clear();
}

QT_END_NAMESPACE
