# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generates the settings.gradle file for the target. Writes the result to the target android build
# directory.
function(_qt_internal_android_generate_bundle_settings_gradle target)
    set(settings_gradle_filename "settings.gradle")
    _qt_internal_android_template_dir(template_directory)
    set(template_file "${template_directory}/${settings_gradle_filename}.in")

    set(android_app_name "$<TARGET_PROPERTY:${target},QT_ANDROID_APP_NAME>")
    string(JOIN "" ROOT_PROJECT_NAME
        "$<IF:$<BOOL:${android_app_name}>,"
            "${android_app_name},"
            "${target}"
        ">"
    )

    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})
    set(settings_gradle_file "${android_build_dir}/${settings_gradle_filename}")

    _qt_internal_configure_file(GENERATE OUTPUT ${settings_gradle_file}
        INPUT "${template_file}")
endfunction()

# Generates the source sets for the target.
function(_qt_internal_android_get_gradle_source_sets out_var target)
    set(known_types java aidl res resources renderscript assets jniLibs)
    set(source_set "")
    set(indent "            ")
    foreach(type IN LISTS known_types)
        set(source_dirs
            "$<TARGET_PROPERTY:${target},_qt_android_gradle_${type}_source_dirs>")
        string(JOIN "" source_set
            "${source_set}"
            "$<$<BOOL:${source_dirs}>:"
                "${indent}${type}.srcDirs = ['$<JOIN:${source_dirs},'$<COMMA> '>']\n"
            ">"
        )
    endforeach()

    set(manifest
        "$<TARGET_PROPERTY:${target},_qt_android_manifest>")
    string(JOIN "" source_set
        "${source_set}"
        "$<$<BOOL:${manifest}>:"
            "${indent}manifest.srcFile '${manifest}'\n"
        ">"
    )
    set(${out_var} "${source_set}" PARENT_SCOPE)
endfunction()

# Generates the gradle dependency list for the target.
function(_qt_internal_android_get_gradle_dependencies out_var target)
    # Use dependencies from file tree by default
    set(known_dependencies
        "implementation fileTree(dir: 'libs', include: ['*.jar', '*.aar'])")
    foreach(dep_type implementation api)
        string(JOIN "\n    " dep_prefix
            "\n    //noinspection GradleDependency"
            "${dep_type} '"
        )
        set(dep_postfix "'")
        set(dep_property "$<TARGET_PROPERTY:${target},_qt_android_gradle_${dep_type}_dependencies>")
        string(JOIN "" known_dependencies
            "${known_dependencies}"
            "$<$<BOOL:${dep_property}>:"
                "${dep_prefix}$<JOIN:${dep_property},${dep_postfix}${dep_prefix}>${dep_postfix}"
            ">"
        )
    endforeach()
    set(${out_var} "${known_dependencies}" PARENT_SCOPE)
endfunction()

# Sets the default values of the gradle properties for the Android executable target.
function(_qt_internal_set_android_application_gradle_defaults target)
    _qt_internal_android_java_dir(android_java_dir)

    set_target_properties(${target} PROPERTIES
        _qt_android_gradle_java_source_dirs "${android_java_dir}/src;src;java"
        _qt_android_gradle_aidl_source_dirs "${android_java_dir}/src;src;aidl"
        _qt_android_gradle_res_source_dirs "${android_java_dir}/res;res"
        _qt_android_gradle_resources_source_dirs "resources"
        _qt_android_gradle_renderscript_source_dirs "src"
        _qt_android_gradle_assets_source_dirs "assets"
        _qt_android_gradle_jniLibs_source_dirs "libs"
        _qt_android_manifest "AndroidManifest.xml"
        _qt_android_gradle_implementation_dependencies "androidx.core:core:1.13.1"
    )
endfunction()

# Generates the build.gradle file for the target. Writes the result to the target app deployment
# directory.
function(_qt_internal_android_generate_target_build_gradle target)
    # TODO: The current build.gradle.in templates hardcodes couple values that needs to be
    # configurable in the future. For example the buildscript dependencies, or the use of
    # androidx.core:core:1.13.1 and the dependency for all user applications.

    _qt_internal_android_get_gradle_property(PACKAGE_NAME ${target}
        QT_ANDROID_PACKAGE_NAME "org.qtproject.example.$<MAKE_C_IDENTIFIER:${target}>")

    _qt_internal_android_get_target_sdk_build_tools_revision(ANDROID_BUILD_TOOLS_VERSION
        ${target})

    _qt_internal_detect_latest_android_platform(ANDROID_COMPILE_SDK_VERSION)
    if(NOT ANDROID_COMPILE_SDK_VERSION)
        message(FATAL_ERROR "Unable to detect the android platform in ${ANDROID_SDK_ROOT}. "
            "Please check your Android SDK installation.")
    endif()

    _qt_internal_android_get_gradle_source_sets(SOURCE_SETS ${target})
    _qt_internal_android_get_gradle_dependencies(GRADLE_DEPENDENCIES ${target})

    _qt_internal_android_get_gradle_property(min_sdk_version ${target}
        QT_ANDROID_MIN_SDK_VERSION "28")

    _qt_internal_android_get_gradle_property(target_sdk_version ${target}
        QT_ANDROID_TARGET_SDK_VERSION "34")

    set(target_abis "$<TARGET_PROPERTY:${target},_qt_android_abis>")
    set(target_abi_list "$<JOIN:${target_abis};${CMAKE_ANDROID_ARCH_ABI},'$<COMMA> '>")

    string(JOIN "\n        " DEFAULT_CONFIG_VALUES
        "resConfig 'en'"
        "minSdkVersion ${min_sdk_version}"
        "targetSdkVersion ${target_sdk_version}"
        "ndk.abiFilters = ['${target_abi_list}']"
    )

    set(ANDROID_DEPLOYMENT_EXTRAS "")

    get_target_property(android_target_type ${target} _qt_android_target_type)
    if(android_target_type STREQUAL "APPLICATION")
        set(GRADLE_PLUGIN_TYPE "com.android.application")
    else()
        message(FATAL_ERROR "Unsupported target type for android bundle deployment ${target}")
    endif()

    _qt_internal_android_get_target_deployment_dir(target_deployment_dir ${target})
    set(build_gradle_filename "build.gradle")
    set(build_gradle_file "${target_deployment_dir}/${build_gradle_filename}")
    _qt_internal_android_template_dir(template_directory)
    _qt_internal_configure_file(GENERATE OUTPUT "${build_gradle_file}"
        INPUT "${template_directory}/${build_gradle_filename}.in")
endfunction()

# Prepares the artifacts for the gradle build of the target.
function(_qt_internal_android_prepare_gradle_build target)
    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})
    _qt_internal_android_get_target_deployment_dir(deployment_dir ${target})

    _qt_internal_android_copy_gradle_files(${target} "${android_build_dir}")

    _qt_internal_android_generate_bundle_gradle_properties(${target})
    _qt_internal_android_generate_bundle_settings_gradle(${target})
    _qt_internal_android_generate_bundle_local_properties(${target})
    _qt_internal_android_generate_target_build_gradle(${target})
    _qt_internal_android_generate_target_gradle_properties(${target})

    set(gradle_scripts
        "${android_build_dir}/gradle.properties"
        "${android_build_dir}/local.properties"
        "${android_build_dir}/settings.gradle"
        "${deployment_dir}/build.gradle"
        "${deployment_dir}/gradle.properties"
    )
    set_target_properties(${target} PROPERTIES _qt_android_deployment_files "${gradle_scripts}")

    _qt_internal_android_add_gradle_build(${target} apk)
    _qt_internal_android_add_gradle_build(${target} aab)

    # Make global apk, aab, and aar targets depend on the respective targets.
    _qt_internal_android_add_global_package_dependencies(${target})
    _qt_internal_create_global_apk_all_target_if_needed()
endfunction()

# Adds the modern gradle build targets.
# These targets use the settings.gradle based build directory structure.
function(_qt_internal_android_add_gradle_build target type)
    _qt_internal_android_get_deployment_type_option(android_deployment_type_option
        "assembleRelease" "assembleDebug")

    _qt_internal_android_gradlew_name(gradlew_file_name)
    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})
    set(gradlew "${android_build_dir}/${gradlew_file_name}")

    set(extra_args "")
    if(type STREQUAL "aab")
        set(extra_args "bundle")
    endif()

    set(package_file_path "${android_build_dir}/${target}.${type}")

    _qt_internal_android_package_path(package_build_dir ${target} ${type})
    _qt_internal_android_get_deployment_type_option(deployment_type_suffix
        "release" "debug")
    set(package_build_file_path
        "${package_build_dir}/${deployment_type_suffix}/app-${deployment_type_suffix}.${type}")

    set(gradle_scripts "$<TARGET_PROPERTY:${target},_qt_android_deployment_files>")
    add_custom_command(OUTPUT "${package_file_path}"
        BYPRODUCTS "${package_build_file_path}"
        COMMAND
            "${gradlew}" ${android_deployment_type_option} ${extra_args}
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            "${package_build_file_path}" "${package_file_path}"
        DEPENDS
            ${target}
            ${gradle_scripts}
            ${target}_copy_gradle_files
            ${target}_android_deploy_aux
        WORKING_DIRECTORY
            "${android_build_dir}"
        VERBATIM
    )

    add_custom_target(${target}_make_${type} DEPENDS "${package_file_path}")
endfunction()

# Returns the path to the android executable package either apk or aab.
function(_qt_internal_android_package_path out_var target type)
    set(supported_package_types apk aab)
    if(NOT type IN_LIST supported_package_types)
        message(FATAL_ERROR "Invalid package type, supported types: ${supported_package_types}")
    endif()

    # aab packages are located in the bundle directory
    if(type STREQUAL "aab")
        set(type "bundle")
    endif()

    _qt_internal_android_get_target_deployment_dir(deployment_dir ${target})

    set(${out_var} "${deployment_dir}/build/outputs/${type}" PARENT_SCOPE)
endfunction()

# Returns the path to the gradle build directory.
function(_qt_internal_android_gradle_template_dir out_var)
    if(PROJECT_NAME STREQUAL "QtBase" OR QT_SUPERBUILD)
        set(${out_var} "${QtBase_SOURCE_DIR}/src/3rdparty/gradle" PARENT_SCOPE)
    else()
        set(${out_var} "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_DATA}/src/3rdparty/gradle" PARENT_SCOPE)
    endif()
endfunction()

# Returns the path to the android java dir.
function(_qt_internal_android_java_dir out_var)
    if(PROJECT_NAME STREQUAL "QtBase" OR QT_SUPERBUILD)
        set(${out_var} "${QtBase_SOURCE_DIR}/src/android/java" PARENT_SCOPE)
    else()
        set(${out_var} "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_DATA}/src/android/java" PARENT_SCOPE)
    endif()
endfunction()

# Returns the platform-spefic name of the gradlew script.
function(_qt_internal_android_gradlew_name out_var)
    if(CMAKE_HOST_WIN32)
        set(gradlew_file_name "gradlew.bat")
    else()
        set(gradlew_file_name "gradlew")
    endif()

    set(${out_var} "${gradlew_file_name}" PARENT_SCOPE)
endfunction()

# Return the path to the gradlew script.
function(_qt_internal_android_gradlew_path out_var target)
    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})
    set(${out_var} "${android_build_dir}/${gradlew_file_name}" PARENT_SCOPE)
endfunction()

# Returns the generator expression for the gradle_property value. Defaults to the default_value
# argument.
function(_qt_internal_android_get_gradle_property out_var target target_property default_value)
    set(target_property_genex "$<GENEX_EVAL:$<TARGET_PROPERTY:${target},${target_property}>>")
    string(JOIN "" result
        "$<IF:$<BOOL:${target_property_genex}>,"
            "${target_property_genex},"
            "${default_value}"
        ">"
    )
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

# Generates gradle.properties for the specific target. Usually contains the
# target build type(executable, dynamic feature, library).
function(_qt_internal_android_generate_target_gradle_properties target)
    set(gradle_properties "gradle.properties")
    _qt_internal_android_template_dir(template_directory)
    set(template_file "${template_directory}/${gradle_properties}.in")
    _qt_internal_android_get_target_deployment_dir(deployment_dir ${target})

    _qt_internal_configure_file(CONFIGURE OUTPUT "${deployment_dir}/${gradle_properties}"
        INPUT "${template_file}")
endfunction()

# Generates the top-level gradle.properties in the android-build directory
# The file contains the information about the versions of the android build
# tools, the list of supported ABIs.
function(_qt_internal_android_generate_bundle_gradle_properties target)
    set(EXTRA_PROPERTIES "")

    set(gradle_properties_file_name "gradle.properties")
    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})
    _qt_internal_android_template_dir(template_directory)
    set(template_file "${template_directory}/${gradle_properties_file_name}.in")
    _qt_internal_configure_file(CONFIGURE
        OUTPUT "${android_build_dir}/${gradle_properties_file_name}"
        INPUT "${template_file}"
    )
endfunction()

# Generates the local.properties for gradle builds. Contains the path to the
# Android SDK root.
function(_qt_internal_android_generate_bundle_local_properties target)
    _qt_internal_android_get_target_android_build_dir(android_build_dir ${target})

    file(TO_CMAKE_PATH "${ANDROID_SDK_ROOT}" ANDROID_SDK_ROOT_NATIVE)
    _qt_internal_configure_file(CONFIGURE OUTPUT "${android_build_dir}/local.properties"
        CONTENT "sdk.dir=${ANDROID_SDK_ROOT_NATIVE}\n")
endfunction()

# Copies gradle scripts to a build directory.
function(_qt_internal_android_copy_gradle_files target output_directory)
    _qt_internal_android_gradlew_name(gradlew_file_name)
    _qt_internal_android_gradle_template_dir(gradle_template_dir)

    set(gradlew_file_src "${gradle_template_dir}/${gradlew_file_name}")
    set(gradlew_file_dst "${output_directory}/${gradlew_file_name}")

    add_custom_command(OUTPUT "${gradlew_file_dst}"
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different "${gradlew_file_src}" "${gradlew_file_dst}"
        DEPENDS "${gradlew_file_src}"
        COMMENT "Copying gradlew script for ${target}"
        VERBATIM
    )

    # TODO: make a more precise directory copying
    set(gradle_dir_src "${gradle_template_dir}/gradle")
    set(gradle_dir_dst "${output_directory}/gradle")
    add_custom_command(OUTPUT "${gradle_dir_dst}"
        COMMAND
            ${CMAKE_COMMAND} -E copy_directory "${gradle_dir_src}" "${gradle_dir_dst}"
        DEPENDS "${gradle_dir_src}"
        COMMENT "Copying gradle support files for ${target}"
        VERBATIM
    )

    add_custom_target(${target}_copy_gradle_files
        DEPENDS
            "${gradlew_file_dst}"
            "${gradle_dir_dst}"
    )
endfunction()
