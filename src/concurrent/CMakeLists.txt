# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## Concurrent Module:
#####################################################################

bobui_internal_add_module(Concurrent
    NO_PRIVATE_MODULE
    EXCEPTIONS
    SOURCES
        bobuiaskbuilder.h
        qtconcurrent_global.h
        bobuiconcurrentcompilertest.h
        bobuiconcurrentfilter.cpp bobuiconcurrentfilter.h
        bobuiconcurrentfilterkernel.h
        bobuiconcurrentfunctionwrappers.h
        bobuiconcurrentiteratekernel.cpp bobuiconcurrentiteratekernel.h
        bobuiconcurrentmap.cpp bobuiconcurrentmap.h
        bobuiconcurrentmapkernel.h
        bobuiconcurrentmedian.h
        bobuiconcurrentreducekernel.h
        bobuiconcurrentrun.cpp qtconcurrentrun.h
        bobuiconcurrentrunbase.h
        bobuiconcurrentstoredfunctioncall.h
        qtconcurrenttask.h
        bobuiconcurrentthreadengine.cpp bobuiconcurrentthreadengine.h
    DEFINES
        BOBUI_NO_CONTEXTLESS_CONNECT
        BOBUI_NO_FOREACH
        BOBUI_NO_URL_CAST_FROM_STRING
        BOBUI_NO_USING_NAMESPACE
        BOBUI_USE_NODISCARD_FILE_OPEN
    LIBRARIES
        BobUI::CorePrivate
    PUBLIC_LIBRARIES
        BobUI::Core
    PRECOMPILED_HEADER
        "../corelib/global/bobui_pch.h"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(Concurrent CONDITION MSVC AND (TEST_architecture_arch STREQUAL "i386")
    LINK_OPTIONS
        "/BASE:0x66000000"
)
bobui_internal_add_docs(Concurrent
    doc/bobuiconcurrent.qdocconf
)
