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
    src_corelib_io_qdatastream.cpp
    src_corelib_io_qdebug.cpp
    src_corelib_io_qdir.cpp
    src_corelib_io_qdiriterator.cpp
    src_corelib_io_qdirlisting.cpp
    src_corelib_io_qfile.cpp
    src_corelib_io_qfileinfo.cpp
    src_corelib_io_qfileselector.cpp
    src_corelib_io_qiodevice.cpp
    src_corelib_io_qprocess.cpp
    src_corelib_io_qsettings.cpp
    src_corelib_io_qstorageinfo.cpp
    src_corelib_io_qtemporarydir.cpp
    src_corelib_io_qtemporaryfile.cpp
    src_corelib_io_qtextstream.cpp
    src_corelib_io_qurl.cpp
    src_corelib_kernel_qabstractitemmodel.cpp
    src_corelib_kernel_qabstractnativeeventfilter_win.cpp
    src_corelib_kernel_qabstractnativeeventfilter.cpp
    src_corelib_kernel_qcoreapplication.cpp
    src_corelib_kernel_qdeadlinetimer.cpp
    src_corelib_kernel_qmath.cpp
    src_corelib_kernel_qmetaobject.cpp
    src_corelib_kernel_qmetatype.cpp
    src_corelib_kernel_qmimedata.cpp
    src_corelib_kernel_qproperty.cpp
    src_corelib_kernel_qsystemsemaphore.cpp
    src_corelib_kernel_qvariant.cpp
)

target_link_libraries(corelib_snippets_code PRIVATE
    Qt::Core
)

qt_internal_extend_target(corelib_snippets_code CONDITION APPLE
    SOURCES
        src_corelib_kernel_qabstractnativeeventfilter.mm
)

qt_internal_extend_target(corelib_snippets_code CONDITION QT_FEATURE_widgets
    LIBRARIES
        Qt::Widgets
    SOURCES
        doc_src_objecttrees.cpp
        src_corelib_animation_qpropertyanimation.cpp
        src_corelib_global_qglobal_widgets.cpp
        src_corelib_kernel_qobject.cpp
        src_corelib_kernel_qtimer.cpp
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
