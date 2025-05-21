# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generates the generator expression that converts the 'target'
# QT_ANDROID_PERMISSIONS property to the specific 'type'.
#
# It's expected that each element in QT_ANDROID_PERMISSIONS list has specific
# format:
# <name>\;<permission>\\\;<extra1>\;<value>\\\;<extra2>\;<value>
#
# Synopsis
#   _qt_internal_android_convert_permissions(out_var target <JSON|XML>)
#
# Arguments
#
# `out_var`
#    The name of the variable where the resulting generator expression is
#    stored.
#
# `target`
#    The name of the target.
#
# `JSON`
#   Generate JSON array known by androiddeployqt.
#
# `XML`
#   Generate XML content compatible with AndroidManifest.xml.
function(_qt_internal_android_convert_permissions out_var target type)
    set(permissions_property "$<TARGET_PROPERTY:${target},QT_ANDROID_PERMISSIONS>")
    set(permissions_genex "$<$<BOOL:${permissions_property}>:")
    if(type STREQUAL "JSON")
        set(pref "{ \"")
        set(post "\" }")
        set(indent "\n      ")
        string(APPEND permissions_genex
            "[${indent}$<JOIN:"
                "$<JOIN:"
                    "${pref}$<JOIN:"
                        "${permissions_property},"
                        "${post}$<COMMA>${indent}${pref}"
                    ">${post},"
                    "\": \""
                ">,"
                "\"$<COMMA> \""
            ">\n    ]"
        )
    elseif(type STREQUAL "XML")
        set(pref "<uses-permission\n android:")
        set(post "' /$<ANGLE-R>\n")
        string(APPEND permissions_genex
            "$<JOIN:"
                "$<JOIN:"
                    "${pref}$<JOIN:"
                        "${permissions_property},"
                        "${post}${pref}"
                    ">${post}\n,"
                    "='"
                ">,"
                "' android:"
            ">"
        )
    else()
        message(FATAL_ERROR "Invalid type ${type}. Supported types: JSON, XML")
    endif()
    string(APPEND permissions_genex ">")

    set(${out_var} "${permissions_genex}" PARENT_SCOPE)
endfunction()
