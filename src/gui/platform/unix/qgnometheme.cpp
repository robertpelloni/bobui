// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qgnometheme_p.h"
#include <qpa/qplatformdialoghelper.h>
#include <qpa/qplatformfontdatabase.h>
#ifndef QT_NO_DBUS
#include "qdbuslistener_p.h"
#include <private/qdbustrayicon_p.h>
#include <private/qdbustrayicon_p.h>
#include <private/qdbusplatformmenu_p.h>
#include <private/qdbusmenubar_p.h>
#endif
#include <qpa/qwindowsysteminterface.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DBUS
Q_STATIC_LOGGING_CATEGORY(lcQpaThemeGnome, "qt.qpa.theme.gnome")
#endif // QT_NO_DBUS

/*!
    \class QGnomeTheme
    \brief QGnomeTheme is a theme implementation for the Gnome desktop.
    \since 5.0
    \internal
    \ingroup qpa
*/
const char *QGnomeTheme::name = "gnome";

QGnomeThemePrivate::QGnomeThemePrivate()
{
#ifndef QT_NO_DBUS
    static constexpr QLatin1String appearanceNamespace("org.freedesktop.appearance");
    static constexpr QLatin1String colorSchemeKey("color-scheme");
    static constexpr QLatin1String contrastKey("contrast");

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (dbus.isConnected()) {
        // ReadAll appears to omit the contrast setting on Ubuntu.
        QDBusMessage message = QDBusMessage::createMethodCall(QLatin1String("org.freedesktop.portal.Desktop"),
                                                              QLatin1String("/org/freedesktop/portal/desktop"),
                                                              QLatin1String("org.freedesktop.portal.Settings"),
                                                              QLatin1String("ReadOne"));

        message << appearanceNamespace << colorSchemeKey;
        QDBusReply<QVariant> reply = dbus.call(message);
        if (Q_LIKELY(reply.isValid())) {
            uint xdgColorSchemeValue = reply.value().toUInt();
            switch (xdgColorSchemeValue) {
            case 1:
                m_colorScheme = Qt::ColorScheme::Dark;
                QWindowSystemInterface::handleThemeChange();
                break;
            case 2:
                m_colorScheme = Qt::ColorScheme::Light;
                QWindowSystemInterface::handleThemeChange();
                break;
            default:
                break;
            }
        }

        message.setArguments({});
        message << appearanceNamespace << contrastKey;
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(dbus.asyncCall(message));
        QObject::connect(watcher, &QDBusPendingCallWatcher::finished, watcher, [this](QDBusPendingCallWatcher *watcher) {
            if (!watcher->isError()) {
                QDBusPendingReply<QVariant> reply = *watcher;
                if (Q_LIKELY(reply.isValid()))
                    updateHighContrast(static_cast<Qt::ContrastPreference>(reply.value().toUInt()));
            }
            initDbus();
            watcher->deleteLater();
        });
    } else {
        qCWarning(lcQpaThemeGnome) << "dbus connection failed. Last error: " << dbus.lastError();
    }
#endif // QT_NO_DBUS
}
QGnomeThemePrivate::~QGnomeThemePrivate()
{
    if (systemFont)
        delete systemFont;
    if (fixedFont)
        delete fixedFont;
}

void QGnomeThemePrivate::configureFonts(const QString &gtkFontName) const
{
    Q_ASSERT(!systemFont);
    const int split = gtkFontName.lastIndexOf(QChar::Space);
    float size = QStringView{gtkFontName}.mid(split + 1).toFloat();
    QString fontName = gtkFontName.left(split);

    systemFont = new QFont(fontName, size);
    fixedFont = new QFont(QLatin1StringView(QGenericUnixTheme::defaultFixedFontNameC), systemFont->pointSize());
    fixedFont->setStyleHint(QFont::TypeWriter);
    qCDebug(lcQpaFonts) << "default fonts: system" << systemFont << "fixed" << fixedFont;
}

#ifndef QT_NO_DBUS
bool QGnomeThemePrivate::initDbus()
{
    dbus.reset(new QDBusListener());
    Q_ASSERT(dbus);

    // Wrap slot in a lambda to avoid inheriting QGnomeThemePrivate from QObject
    auto wrapper = [this](QDBusListener::Provider provider,
                          QDBusListener::Setting setting,
                          const QVariant &value) {
        if (provider != QDBusListener::Provider::Gnome
            && provider != QDBusListener::Provider::Gtk) {
            return;
        }

        switch (setting) {
        case QDBusListener::Setting::Theme:
            updateColorScheme(value.toString());
            break;
        case QDBusListener::Setting::Contrast:
            updateHighContrast(static_cast<Qt::ContrastPreference>(value.toUInt()));
            break;
        default:
            break;
        }
    };

    return QObject::connect(dbus.get(), &QDBusListener::settingChanged, dbus.get(), wrapper);
}

void QGnomeThemePrivate::updateColorScheme(const QString &themeName)
{
    const auto oldColorScheme = m_colorScheme;
    if (themeName.contains(QLatin1StringView("light"), Qt::CaseInsensitive)) {
        m_colorScheme = Qt::ColorScheme::Light;
    } else if (themeName.contains(QLatin1StringView("dark"), Qt::CaseInsensitive)) {
        m_colorScheme = Qt::ColorScheme::Dark;
    } else {
        m_colorScheme = Qt::ColorScheme::Unknown;
    }

    if (oldColorScheme != m_colorScheme)
        QWindowSystemInterface::handleThemeChange();
}

void QGnomeThemePrivate::updateHighContrast(Qt::ContrastPreference contrast)
{
    if (m_contrast == contrast)
        return;
    m_contrast = contrast;
    QWindowSystemInterface::handleThemeChange();
}

#endif // QT_NO_DBUS

QGnomeTheme::QGnomeTheme()
    : QGenericUnixTheme(new QGnomeThemePrivate())
{
}

QVariant QGnomeTheme::themeHint(QPlatformTheme::ThemeHint hint) const
{
    switch (hint) {
    case QPlatformTheme::DialogButtonBoxButtonsHaveIcons:
        return QVariant(true);
    case QPlatformTheme::DialogButtonBoxLayout:
        return QVariant(QPlatformDialogHelper::GnomeLayout);
    case QPlatformTheme::SystemIconThemeName:
        return QVariant(QStringLiteral("Adwaita"));
    case QPlatformTheme::SystemIconFallbackThemeName:
        return QVariant(QStringLiteral("gnome"));
    case QPlatformTheme::IconThemeSearchPaths:
        return QVariant(xdgIconThemePaths());
    case QPlatformTheme::IconPixmapSizes:
        return QVariant::fromValue(availableXdgFileIconSizes());
    case QPlatformTheme::StyleNames: {
        QStringList styleNames;
        styleNames << QStringLiteral("Fusion") << QStringLiteral("windows");
        return QVariant(styleNames);
    }
    case QPlatformTheme::KeyboardScheme:
        return QVariant(int(GnomeKeyboardScheme));
    case QPlatformTheme::PasswordMaskCharacter:
        return QVariant(QChar(0x2022));
    case QPlatformTheme::UiEffects:
        return QVariant(int(HoverEffect));
    case QPlatformTheme::ButtonPressKeys:
        return QVariant::fromValue(
                QList<Qt::Key>({ Qt::Key_Space, Qt::Key_Return, Qt::Key_Enter, Qt::Key_Select }));
    case QPlatformTheme::PreselectFirstFileInDirectory:
        return true;
    case QPlatformTheme::MouseCursorTheme:
        return QVariant(mouseCursorTheme());
    case QPlatformTheme::MouseCursorSize:
        return QVariant(mouseCursorSize());
    case QPlatformTheme::PreferFileIconFromTheme:
        return true;
    default:
        break;
    }
    return QPlatformTheme::themeHint(hint);
}

QIcon QGnomeTheme::fileIcon(const QFileInfo &fileInfo, QPlatformTheme::IconOptions) const
{
#if QT_CONFIG(mimetype)
    return xdgFileIcon(fileInfo);
#else
    Q_UNUSED(fileInfo);
    return QIcon();
#endif
}

const QFont *QGnomeTheme::font(Font type) const
{
    Q_D(const QGnomeTheme);
    if (!d->systemFont)
        d->configureFonts(gtkFontName());
    switch (type) {
    case QPlatformTheme::SystemFont:
        return d->systemFont;
    case QPlatformTheme::FixedFont:
        return d->fixedFont;
    default:
        return nullptr;
    }
}

QString QGnomeTheme::gtkFontName() const
{
    return QStringLiteral("%1 %2").arg(QLatin1StringView(defaultSystemFontNameC))
                                  .arg(defaultSystemFontSize);
}

#ifndef QT_NO_DBUS
QPlatformMenuBar *QGnomeTheme::createPlatformMenuBar() const
{
    if (isDBusGlobalMenuAvailable())
        return new QDBusMenuBar();
    return nullptr;
}

Qt::ColorScheme QGnomeTheme::colorScheme() const
{
    return d_func()->m_colorScheme;
}

Qt::ContrastPreference QGnomeTheme::contrastPreference() const
{
    return d_func()->m_contrast;
}

#endif

#if !defined(QT_NO_DBUS) && !defined(QT_NO_SYSTEMTRAYICON)
QPlatformSystemTrayIcon *QGnomeTheme::createPlatformSystemTrayIcon() const
{
    if (shouldUseDBusTray())
        return new QDBusTrayIcon();
    return nullptr;
}
#endif

QString QGnomeTheme::standardButtonText(int button) const
{
    switch (button) {
    case QPlatformDialogHelper::Ok:
        return QCoreApplication::translate("QGnomeTheme", "&OK");
    case QPlatformDialogHelper::Save:
        return QCoreApplication::translate("QGnomeTheme", "&Save");
    case QPlatformDialogHelper::Cancel:
        return QCoreApplication::translate("QGnomeTheme", "&Cancel");
    case QPlatformDialogHelper::Close:
        return QCoreApplication::translate("QGnomeTheme", "&Close");
    case QPlatformDialogHelper::Discard:
        return QCoreApplication::translate("QGnomeTheme", "Close without Saving");
    default:
        break;
    }
    return QPlatformTheme::standardButtonText(button);
}

QT_END_NAMESPACE
