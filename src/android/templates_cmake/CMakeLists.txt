# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(templates_files
    "${CMAKE_CURRENT_SOURCE_DIR}/settings.gradle.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/build.gradle.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/gradle.properties.in"
)

add_custom_target(Qt6AndroidCMakeTemplates
    SOURCES
        ${templates_files}
)

qt_path_join(destination ${QT_INSTALL_DIR} ${INSTALL_DATADIR} "src/android/templates_cmake")

qt_copy_or_install(FILES ${templates_files} DESTINATION "${destination}")

if(NOT QT_WILL_INSTALL)
    qt_internal_copy_at_build_time(TARGET Qt6AndroidCMakeTemplates
        FILES ${templates_files}
        DESTINATION ${destination}
    )
endif()
