cmake_minimum_required(VERSION 3.16)

project(HardwareDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick SerialPort Bluetooth)

include_directories(../../core/include)
# We point to the local deps folder for the mock JUCE headers
include_directories(../../deps/juce)

add_executable(HardwareDemo
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
    ../../core/src/hardware/OmniSerialPort.cpp
    ../../core/src/hardware/OmniBluetooth.cpp
    ../../core/src/desktop/OmniSystemTray.cpp
    ../../core/src/desktop/OmniClipboard.cpp
    ../../core/src/desktop/OmniDragDrop.cpp
    ../../core/src/theme/OmniThemeManager.cpp
    ../../deps/juce/juce_mock.cpp
)

target_link_libraries(HardwareDemo PRIVATE
    BobUI6::Core BobUI6::Gui BobUI6::Widgets BobUI6::Qml BobUI6::Quick
    BobUI6::SerialPort BobUI6::Bluetooth
)
