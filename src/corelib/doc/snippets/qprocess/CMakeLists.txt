add_library(snippets_qprocess OBJECT
    qprocess-createprocessargumentsmodifier.cpp
    qprocess-simpleexecution.cpp
)

target_link_libraries(snippets_qprocess PRIVATE
    Qt::Core
)
