# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## Xml Module:
#####################################################################

bobui_internal_add_module(Xml
    SOURCES
        qtxmlglobal.h
        BobUIXml
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_USING_NAMESPACE
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_USE_NODISCARD_FILE_OPEN
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
    PRIVATE_MODULE_INTERFACE
        BobUI::CorePrivate
)

bobui_internal_extend_target(Xml CONDITION BOBUI_FEATURE_dom
    SOURCES
        compat/removed_api.cpp
        dom/qdom.cpp dom/qdom.h dom/qdom_p.h
        dom/qdomhelpers.cpp dom/qdomhelpers_p.h
    NO_PCH_SOURCES
        compat/removed_api.cpp
)

bobui_internal_extend_target(Xml CONDITION MSVC AND (TEST_architecture_arch STREQUAL "i386")
    LINK_OPTIONS
        "/BASE:0x61000000"
)

bobui_internal_add_docs(Xml
    doc/bobuixml.qdocconf
)

