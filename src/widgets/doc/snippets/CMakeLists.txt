# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

add_library(widgets_snippets OBJECT
    customstyle/customstyle.cpp
    customstyle/main.cpp
    customviewstyle/customviewstyle.cpp
    dialogs/dialogs.cpp
    dockwidgets/mainwindow.cpp
    itemselection/main.cpp
    layouts/layouts.cpp
    qlistview-dnd/mainwindow.cpp
    qlistview-dnd/model.cpp
    qlistwidget-dnd/mainwindow.cpp
    qsortfilterproxymodel/main.cpp
    qsplashscreen/main.cpp
    qstackedlayout/main.cpp
    qstackedwidget/main.cpp
    qstyleoption/main.cpp
    qstyleplugin/main.cpp
    qtablewidget-resizing/mainwindow.cpp
    qtablewidget-using/mainwindow.cpp
    qtreewidget-using/mainwindow.cpp
    qtreewidgetitemiterator-using/mainwindow.cpp
    reading-selections/window.cpp
    shareddirmodel/main.cpp
    simplemodel-use/main.cpp
    splitter/splitter.cpp
    stringlistmodel/main.cpp
    stringlistmodel/model.cpp
    styles/styles.cpp
    textdocument-imagedrop/textedit.cpp
    timeline/main.cpp
    tooltips/main.cpp
    updating-selections/window.cpp
    whatsthis/whatsthis.cpp
    widget-mask/main.cpp
    widgets-tutorial/template.cpp
    mainwindowsnippet.cpp
    widgetdelegate.cpp
)

target_link_libraries(widgets_snippets PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

qt_internal_extend_target(widgets_snippets
    LIBRARIES
        Qt::GuiPrivate
    SOURCES
        qrhiwidget/rhiwidgetintro.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_filedialog
    SOURCES
        filedialogurls/filedialogurls.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_graphicsview
    SOURCES
        graphicssceneadditem/graphicssceneadditemsnippet.cpp
        graphicsview/graphicsview_snippet.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_opengl AND QT_FEATURE_printsupport
    LIBRARIES
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::PrintSupport
    SOURCES
        graphicsview/graphicsview.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_mdiarea
    SOURCES
        mdiarea/mdiareasnippets.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_scrollarea
    SOURCES
        myscrollarea/myscrollarea.cpp
)

qt_internal_extend_target(widgets_snippets CONDITION QT_FEATURE_spinbox AND QT_FEATURE_itemviews
    SOURCES
        qitemdelegate/spinbox-delegate.cpp
)

set_target_properties(widgets_snippets PROPERTIES COMPILE_OPTIONS "-w")

if ("${CMAKE_CXX_COMPILE_FEATURES}" MATCHES "cxx_std_23")
    set_property(TARGET widgets_snippets PROPERTY CXX_STANDARD 23)
endif()

set_target_properties(widgets_snippets PROPERTIES UNITY_BUILD OFF)
