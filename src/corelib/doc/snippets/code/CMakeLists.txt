add_library(corelib_snippets_code OBJECT
    doc_src_containers.cpp
    doc_src_properties.cpp
    doc_src_qalgorithms.cpp
    doc_src_qcache.cpp
    doc_src_qiterator.cpp
    doc_src_qnamespace.cpp
    doc_src_qplugin.cpp
    doc_src_qset.cpp
    doc_src_qvarlengtharray.cpp
    doc_src_resources.cpp
    src_corelib_animation_qparallelanimationgroup.cpp
    src_corelib_animation_qsequentialanimationgroup.cpp
    src_corelib_animation_qvariantanimation.cpp
    src_corelib_concurrent_qthreadpool.cpp
    src_corelib_global_qglobal.cpp
    src_corelib_global_qlogging.cpp
    src_corelib_global_qnumeric.cpp
    src_corelib_global_qoperatingsystemversion.cpp
    src_corelib_global_qrandom.cpp
)

target_link_libraries(corelib_snippets_code PRIVATE
    Qt::Core
)

qt_internal_extend_target(corelib_snippets_code CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCES
        doc_src_objecttrees.cpp
        src_corelib_animation_qpropertyanimation.cpp
        src_corelib_global_qglobal_widgets.cpp
)

qt_internal_extend_target(corelib_snippets_code CONDITION QT_FEATURE_gui
    LIBRARIES
        Qt::Gui
)

qt_internal_extend_target(corelib_snippets_code CONDITION QT_FEATURE_network
    LIBRARIES
        Qt::Network
)

set_target_properties(corelib_snippets_code PROPERTIES COMPILE_OPTIONS "-w")
