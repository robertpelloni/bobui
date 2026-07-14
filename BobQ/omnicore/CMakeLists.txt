# Placeholder for the future build logic that compiles omnicore components
# We must ensure BobQBox.cpp and BobQGrid.cpp are included in the build target.

add_library(OmniCore STATIC
    src/BobQJuceHost.cpp
    src/BobQBox.cpp
    src/BobQGrid.cpp
    src/BobQUltimatePPHost.cpp
    src/OmniMasterClock.cpp
    src/BobQUppComponentRegistry.cpp
)
target_include_directories(OmniCore PUBLIC include)

# Link JUCE Submodule if available
if(TARGET juce::juce_gui_basics)
    target_link_libraries(OmniCore PUBLIC juce::juce_gui_basics)
endif()

# Qt Dependencies
find_package(Qt6 COMPONENTS Quick REQUIRED)
target_link_libraries(OmniCore PUBLIC Qt6::Quick)
