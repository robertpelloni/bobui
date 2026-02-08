cmake_minimum_required(VERSION 3.16)

project(Global3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(Global3D
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/widgets/OmniSlider.cpp
    ../../core/src/widgets/OmniTextField.cpp
    ../../core/src/widgets/OmniDial.cpp
    ../../core/src/widgets/OmniCheckBox.cpp
    ../../core/src/widgets/OmniComboBox.cpp
    ../../core/src/widgets/OmniProgressBar.cpp
    ../../core/src/charts/OmniLineChart.cpp
    ../../core/src/charts/OmniOscilloscope.cpp
    ../../core/src/layout/OmniDockArea.cpp
    ../../core/src/layout/OmniSplitView.cpp
    ../../core/src/net/OmniHttpClient.cpp
    ../../core/src/net/OmniWebSocket.cpp
    ../../core/src/audio/dsp/OmniGain.cpp
    ../../core/src/audio/dsp/OmniFilter.cpp
    ../../core/src/plugin/OmniPluginManager.cpp
    ../../core/src/data/OmniDatabase.cpp
    ../../core/src/gfx/OmniShaderEffect.cpp
    ../../core/src/gfx/OmniParticles.cpp
    ../../core/src/widgets/OmniCodeEditor.cpp
    ../../core/src/widgets/OmniCalendar.cpp
    ../../core/src/i18n/OmniTranslator.cpp
    ../../core/src/3d/OmniView3D.cpp
    ../../core/src/3d/OmniMesh.cpp
    ../../core/src/3d/OmniLight.cpp
    ../../core/src/test/OmniTestCase.cpp
    ../../core/src/test/OmniTestRunner.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(Global3D PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)

if(EMSCRIPTEN)
    target_link_options(HelloOmni PRIVATE -s USE_PTHREADS=0)
endif()
