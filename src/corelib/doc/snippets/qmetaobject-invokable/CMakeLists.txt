if(NOT QT_FEATURE_widgets)
    return()
endif()

add_library(corelib_snippets_qmetaobject-invokable OBJECT)

target_link_libraries(corelib_snippets_qmetaobject-invokable PRIVATE
    Qt::Core
)

qt_internal_extend_target(corelib_snippets_qmetaobject-invokable CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCES
        main.cpp
        window.cpp
)
