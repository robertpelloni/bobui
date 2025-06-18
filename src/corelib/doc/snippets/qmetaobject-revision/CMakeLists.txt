if(NOT QT_FEATURE_widgets)
    return()
endif()

add_library(snippets_qmetaobject-revision OBJECT)

target_link_libraries(snippets_qmetaobject-revision PRIVATE
    Qt::Core
)

qt_internal_extend_target(snippets_qmetaobject-revision CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCES
        main.cpp
        window.cpp
)
