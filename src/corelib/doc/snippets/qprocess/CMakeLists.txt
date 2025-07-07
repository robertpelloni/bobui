set(CMAKE_UNITY_BUILD OFF)

add_library(corelib_snippets_qprocess OBJECT
    qprocess-createprocessargumentsmodifier.cpp
    qprocess-simpleexecution.cpp
)

target_link_libraries(corelib_snippets_qprocess PRIVATE
    Qt::Core
)
