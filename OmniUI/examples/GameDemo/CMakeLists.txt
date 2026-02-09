cmake_minimum_required(VERSION 3.16)

project(GameDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(GameDemo
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
    ../../core/src/game/OmniSprite.cpp
    ../../core/src/game/OmniInputManager.cpp
    ../../core/src/physics/OmniPhysicsWorld.cpp
    ../../core/src/physics/OmniRigidBody.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(GameDemo PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)

if(EMSCRIPTEN)
    target_link_options(HelloOmni PRIVATE -s USE_PTHREADS=0)
endif()
