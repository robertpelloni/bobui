# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

add_library(gui_snippets OBJECT
    brush/brush.cpp
    brush/gradientcreationsnippet.cpp
    image/image.cpp
    image/supportedformat.cpp
    picture/picture.cpp
    polygon/polygon.cpp
    qimagewriter/main.cpp
    qtextobject/textobjectinterface.h
    textdocument-end/textdocumentendsnippet.cpp
)

target_link_libraries(gui_snippets PRIVATE
    Qt::Core
    Qt::Gui
)

qt_internal_extend_target(gui_snippets CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCES
        draganddrop/dragwidget.cpp
        dragging/mainwindow.cpp
        droparea/droparea.cpp
        dropevents/window.cpp
        droprectangle/window.cpp
        plaintextlayout/window.cpp
        qfileopenevent/main.cpp
        qfontdatabase/qfontdatabase_snippets.cpp
        qstatustipevent/main.cpp
        scribe-overview/main.cpp
        separations/finalwidget.cpp
        textblock-formats/main.cpp
        textdocument-blocks/mainwindow.cpp
        textdocument-charformats/main.cpp
        textdocument-css/main.cpp
        textdocument-cursors/main.cpp
        textdocument-find/main.cpp
        textdocument-frames/mainwindow.cpp
        textdocument-imageformat/main.cpp
        textdocument-images/main.cpp
        textdocument-listitems/mainwindow.cpp
        textdocument-listitemstyles/mainwindow.cpp
        textdocument-lists/mainwindow.cpp
        textdocument-resources/main.cpp
        textdocument-selections/mainwindow.cpp
        textdocument-tables/mainwindow.cpp
        textdocument-tables/mainwindow.cpp
        transform/main.cpp
)

qt_internal_extend_target(gui_snippets
    LIBRARIES
        Qt::GuiPrivate
    SOURCES
        rhioffscreen/main.cpp
)

qt_internal_extend_target(gui_snippets CONDITION QT_FEATURE_xml
    LIBRARIES
        Qt::Xml
    SOURCES
        textblock-fragments/xmlwriter.cpp
)

qt_internal_extend_target(gui_snippets CONDITION QT_FEATURE_printsupport
    LIBRARIES
        Qt::PrintSupport
    SOURCES
        textdocument-printing/mainwindow.cpp
)

qt_internal_extend_target(gui_snippets CONDITION QT_FEATURE_clipboard
    SOURCES
        clipboard/clipwindow.cpp
)

set_target_properties(gui_snippets PROPERTIES COMPILE_OPTIONS "-w")

if ("${CMAKE_CXX_COMPILE_FEATURES}" MATCHES "cxx_std_23")
    set_property(TARGET gui_snippets PROPERTY CXX_STANDARD 23)
endif()

set_target_properties(gui_snippets PROPERTIES UNITY_BUILD OFF)
