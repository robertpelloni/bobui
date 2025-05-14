# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(deployment_settings)

if(QT_USE_TARGET_ANDROID_BUILD_DIR)
    add_subdirectory(package_source_dir)
endif()
