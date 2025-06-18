add_library(snippets_eventfilters OBJECT)

target_link_libraries(snippets_eventfilters PRIVATE
    Qt::Core
)

qt_internal_extend_target(snippets_eventfilters CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCE
        main.cpp
)

qt_internal_extend_target(snippets_eventfilters CONDITION QT_FEATURE_gui
    LIBRARIES
        Qt::Gui
    SOURCES
        filterobject.cpp
)
