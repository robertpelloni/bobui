if(NOT TARGET Qt::Widgets)
    return()
endif()

add_library(snippets_qsignalmapper OBJECT
    buttonwidget.cpp
)

add_compile_definitions(EXAMPLE_ONE)

target_link_libraries(snippets_qsignalmapper PRIVATE
    Qt::Core
)

qt_internal_extend_target(snippets_qsignalmapper CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
)
