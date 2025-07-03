# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT INTEGRITY AND TARGET Qt6::Network AND TARGET Qt6::Concurrent)
    add_subdirectory(downloader)
endif()
