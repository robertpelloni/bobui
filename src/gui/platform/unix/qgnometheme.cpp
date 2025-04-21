// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qgnometheme_p.h"
#include <qpa/qplatformdialoghelper.h>
#include <qpa/qplatformfontdatabase.h>
#if QT_CONFIG(dbus)
#include "qdbuslistener_p.h"
#include <private/qdbustrayicon_p.h>
#include <private/qdbustrayicon_p.h>
#include <private/qdbusplatformmenu_p.h>
#include <private/qdbusmenubar_p.h>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusPendingCall>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusVariant>
#endif
#include <qpa/qwindowsysteminterface.h>

QT_BEGIN_NAMESPACE

#if QT_CONFIG(dbus)
Q_STATIC_LOGGING_CATEGORY(lcQpaThemeGnome, "qt.qpa.theme.gnome")

using namespace Qt::StringLiterals;

namespace {
// https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.portal.Settings.html
enum class XDG_ColorScheme : uint { NoPreference, PreferDark, PreferLight };

constexpr Qt::ColorScheme convertColorScheme(XDG_ColorScheme colorScheme)
{
    switch (colorScheme) {
    case XDG_ColorScheme::NoPreference:
        return Qt::ColorScheme::Unknown;
    case XDG_ColorScheme::PreferDark:
        return Qt::ColorScheme::Dark;
    case XDG_ColorScheme::PreferLight:
        return Qt::ColorScheme::Light;
    default:
        Q_UNREACHABLE_RETURN(Qt::ColorScheme::Unknown);
        break;
    }
}

constexpr XDG_ColorScheme convertColorScheme(Qt::ColorScheme colorScheme)
{
    switch (colorScheme) {
    case Qt::ColorScheme::Unknown:
        return XDG_ColorScheme::NoPreference;
    case Qt::ColorScheme::Light:
        return XDG_ColorScheme::PreferLight;
    case Qt::ColorScheme::Dark:
        return XDG_ColorScheme::PreferDark;
    default:
        Q_UNREACHABLE_RETURN(XDG_ColorScheme::NoPreference);
        break;
    }
}

class DBusInterface
{
    DBusInterface() = delete;

    constexpr static auto Service = "org.freedesktop.portal.Desktop"_L1;
    constexpr static auto Path = "/org/freedesktop/portal/desktop"_L1;

public:
    static inline QVariant query(QLatin1StringView interface, QLatin1StringView method,
                                 QLatin1StringView name_space, QLatin1StringView key);
    static inline uint queryPortalVersion();
    static inline QLatin1StringView readOneMethod();
    static inline std::optional<Qt::ColorScheme> queryColorScheme();
    static inline std::optional<Qt::ContrastPreference> queryContrast();
};

QVariant DBusInterface::query(QLatin1StringView interface, QLatin1StringView method,
                              QLatin1StringView name_space, QLatin1StringView key)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (dbus.isConnected()) {
        QDBusMessage message = QDBusMessage::createMethodCall(
                DBusInterface::Service, DBusInterface::Path, interface, method);
        message << name_space << key;

        QDBusReply<QVariant> reply = dbus.call(message);
        if (Q_LIKELY(reply.isValid()))
            return reply.value();
    } else {
        qCWarning(lcQpaThemeGnome) << "dbus connection failed. Last error: " << dbus.lastError();
    }

    return {};
}

uint DBusInterface::queryPortalVersion()
{
    constexpr auto interface = "org.freedesktop.DBus.Properties"_L1;
    constexpr auto method = "Get"_L1;
    constexpr auto name_space = "org.freedesktop.portal.Settings"_L1;
    constexpr auto key = "version"_L1;

    static uint version = 0; // cached version value

    if (version == 0) {
        QVariant reply = query(interface, method, name_space, key);
        if (reply.isValid())
            version = reply.toUInt(); // caches the value for the next calls
    }

    return version;
}

QLatin1StringView DBusInterface::readOneMethod()
{
    // Based on the documentation on flatpak:
    // https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.portal.Settings.html
    // The method name "Read" has changed to "ReadOne" since version 2.
    const uint version = queryPortalVersion();
    if (version == 1)
        return "Read"_L1;
    return "ReadOne"_L1;
}

std::optional<Qt::ColorScheme> DBusInterface::queryColorScheme()
{
    constexpr auto interface = "org.freedesktop.portal.Settings"_L1;
    constexpr auto name_space = "org.freedesktop.appearance"_L1;
    constexpr auto key = "color-scheme"_L1;
    const auto method = readOneMethod();

    QVariant reply = query(interface, method, name_space, key);
    if (reply.isValid())
        return convertColorScheme(
                XDG_ColorScheme{ reply.value<QDBusVariant>().variant().toUInt() });

    return {};
}

std::optional<Qt::ContrastPreference> DBusInterface::queryContrast()
{
    constexpr auto interface = "org.freedesktop.portal.Settings"_L1;
    const auto method = readOneMethod();

    constexpr auto namespace_xdg_portal = "org.freedesktop.appearance"_L1;
    constexpr auto key_xdg_portal = "contrast"_L1;
    QVariant reply = query(interface, method, namespace_xdg_portal, key_xdg_portal);
    if (reply.isValid())
        return static_cast<Qt::ContrastPreference>(reply.toUInt());

    // Fall back to desktop-specific methods (GSettings for GNOME)
    constexpr auto namespace_gsettings = "org.gnome.desktop.a11y.interface"_L1;
    constexpr auto key_gsettings = "high-contrast"_L1;
    reply = query(interface, method, namespace_gsettings, key_gsettings);
    if (reply.isValid())
        return reply.toBool() ? Qt::ContrastPreference::HighContrast
                              : Qt::ContrastPreference::NoPreference;

    return {};
}
} // namespace

#endif // QT_CONFIG(dbus)

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
#if QT_CONFIG(dbus)
    initDbus();

    if (auto value = DBusInterface::queryColorScheme(); value.has_value())
        updateColorScheme(value.value());

    if (auto value = DBusInterface::queryContrast(); value.has_value())
        updateHighContrast(value.value());
#endif // QT_CONFIG(dbus)
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

#if QT_CONFIG(dbus)
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
        case QDBusListener::Setting::ColorScheme:
            updateColorScheme(convertColorScheme(XDG_ColorScheme{ value.toUInt() }));
            break;
        case QDBusListener::Setting::Theme:
            m_themeName = value.toString();
            break;
        case QDBusListener::Setting::Contrast:
            updateHighContrast(value.value<Qt::ContrastPreference>());
            break;
        default:
            break;
        }
    };

    return QObject::connect(dbus.get(), &QDBusListener::settingChanged, dbus.get(), wrapper);
}

Qt::ColorScheme QGnomeThemePrivate::colorScheme() const
{
    if (m_colorScheme != Qt::ColorScheme::Unknown)
        return m_colorScheme;

    // If the color scheme is set to Unknown by mistake or is not set at all,
    // then maybe the theme name contains a hint about the color scheme.
    // Let's hope the theme name does not include any accent color name
    // which contains "dark" or "light" in it (e.g. lightblue). At the moment they don't.
    if (m_themeName.contains(QLatin1StringView("light"), Qt::CaseInsensitive))
        return Qt::ColorScheme::Light;
    else if (m_themeName.contains(QLatin1StringView("dark"), Qt::CaseInsensitive))
        return Qt::ColorScheme::Dark;
    else
        return Qt::ColorScheme::Unknown;
}

void QGnomeThemePrivate::updateColorScheme(Qt::ColorScheme colorScheme)
{
    if (m_colorScheme == colorScheme)
        return;
    m_colorScheme = colorScheme;
    QWindowSystemInterface::handleThemeChange();
}

void QGnomeThemePrivate::updateHighContrast(Qt::ContrastPreference contrast)
{
    if (m_contrast == contrast)
        return;
    m_contrast = contrast;
    QWindowSystemInterface::handleThemeChange();
}

#endif // QT_CONFIG(dbus)

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

#if QT_CONFIG(dbus)
QPlatformMenuBar *QGnomeTheme::createPlatformMenuBar() const
{
    if (isDBusGlobalMenuAvailable())
        return new QDBusMenuBar();
    return nullptr;
}

Qt::ColorScheme QGnomeTheme::colorScheme() const
{
    return d_func()->colorScheme();
}

Qt::ContrastPreference QGnomeTheme::contrastPreference() const
{
    return d_func()->m_contrast;
}

#endif

#if QT_CONFIG(dbus) && QT_CONFIG(systemtrayicon)
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
