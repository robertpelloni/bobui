// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGNOMETHEME_P_H
#define QGNOMETHEME_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qgenericunixtheme_p.h"
#include <qpa/qplatformtheme.h>
#include <qpa/qplatformtheme_p.h>
#include <QtGui/QFont>

QT_BEGIN_NAMESPACE
class QGnomeThemePrivate;
#if QT_CONFIG(dbus)
class QDBusListener;
#endif

class Q_GUI_EXPORT QGnomeTheme : public QGenericUnixTheme
{
    Q_DECLARE_PRIVATE(QGnomeTheme)
public:
    QGnomeTheme();
    QVariant themeHint(ThemeHint hint) const override;
    QIcon fileIcon(const QFileInfo &fileInfo,
                   QPlatformTheme::IconOptions = { }) const override;
    const QFont *font(Font type) const override;
    QString standardButtonText(int button) const override;

    virtual QString gtkFontName() const;
#if QT_CONFIG(dbus)
    QPlatformMenuBar *createPlatformMenuBar() const override;
    Qt::ColorScheme colorScheme() const override;
    Qt::ContrastPreference contrastPreference() const override;
#endif
#if QT_CONFIG(dbus) && QT_CONFIG(systemtrayicon)
    QPlatformSystemTrayIcon *createPlatformSystemTrayIcon() const override;
#endif

    static const char *name;
};

class QGnomeThemePrivate : public QGenericUnixThemePrivate
{
public:
    QGnomeThemePrivate();
    ~QGnomeThemePrivate();

    void configureFonts(const QString &gtkFontName) const;

    mutable QFont *systemFont = nullptr;
    mutable QFont *fixedFont = nullptr;

#if QT_CONFIG(dbus)
    Qt::ColorScheme m_colorScheme = Qt::ColorScheme::Unknown;
    Qt::ContrastPreference m_contrast = Qt::ContrastPreference::NoPreference;
private:
    std::unique_ptr<QDBusListener> dbus;
    bool initDbus();
    void updateColorScheme(const QString &themeName);
    void updateHighContrast(Qt::ContrastPreference contrast);
#endif // QT_CONFIG(dbus)
};


QT_END_NAMESPACE
#endif // QGNOMETHEME_P_H
