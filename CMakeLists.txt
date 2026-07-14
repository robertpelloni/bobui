# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Need an explicit call at the top level. This is the absolute minimum version
# needed to configure the project with any combination of enabled features.
# The call to bobui_build_repo_begin() will upgrade policies further.
#
# The absolute minimum version for building BobUI is 3.16 because for metatype.json generation, we
# depend on being able to find the location of json files created by AUTOMOC/moc.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.md" PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
string(REGEX REPLACE "^v" "" PROJECT_VERSION "${PROJECT_VERSION}")

cmake_minimum_required(VERSION 3.16)

# Get the repo version and CMake policy details
include(.cmake.conf)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobUIBaseHelpers.cmake)

bobui_internal_check_if_path_has_symlinks("${CMAKE_BINARY_DIR}")

# This needs to be called before the first project() call.
bobui_internal_bobuibase_run_autodetect()

# This call will load any provided cmake toolchain file.
project(BobUIBase
    VERSION "${PROJECT_VERSION}"
    DESCRIPTION "BobUI Base Libraries"
    HOMEPAGE_URL "https://bobui.io/"
    LANGUAGES CXX C
)

if(UNIX AND NOT ANDROID)
  enable_language(ASM)
endif()

set(BOBUI_BUILD_EXTRA_IDE_FILE_PATTERNS bin/* libexec/*)

bobui_internal_bobuibase_build_repo()


# BobQ Bridge: Safely load external submodules (JUCE, Ultimate++)
# This must run AFTER qt_internal_qtbase_build_repo to ensure Qt's strict
# environments are fully established before third-party code pollutes them.
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BobQSubmodules.cmake)
bobq_integrate_submodules()
