add_library(corelib_snippets_eventfilters OBJECT)

set_target_properties(corelib_snippets_eventfilters PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(corelib_snippets_eventfilters PRIVATE
    Qt::Core
)

qt_internal_extend_target(corelib_snippets_eventfilters CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCE
        main.cpp
)

qt_internal_extend_target(corelib_snippets_eventfilters CONDITION QT_FEATURE_gui
    LIBRARIES
        Qt::Gui
    SOURCES
        filterobject.cpp
)
