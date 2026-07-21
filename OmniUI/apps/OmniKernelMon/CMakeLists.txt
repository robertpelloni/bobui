cmake_minimum_required(VERSION 3.16)

project(OmniKernelMon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

include_directories(../../core/include)
include_directories(../../deps/juce)

add_executable(OmniKernelMon
    src/main.cpp
    ../../core/src/OmniApp.cpp
    ../../core/src/JuceWidget.cpp
    ../../core/src/kernel/OmniKernel.cpp
    ../../core/src/kernel/OmniProcess.cpp
    ../../core/src/kernel/OmniScheduler.cpp
    ../../core/src/kernel/OmnieBPF.cpp
    ../../core/src/charts/OmniOscilloscope.cpp
    ../../core/src/widgets/OmniButton.cpp
    ../../core/src/widgets/OmniProgressBar.cpp
    ../../core/src/debug/OmniCrashHandler.cpp
    ../../core/src/devtools/OmniConsole.cpp
    ../../core/src/system/OmniSystem.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(OmniKernelMon PRIVATE
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick
)
