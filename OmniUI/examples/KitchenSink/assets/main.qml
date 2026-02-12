import QtQuick 2.15
import QtQuick.Layouts 1.15
import OmniUI 1.0

import OmniAudio 1.0
import OmniNodes 1.0
import OmniCharts 1.0
import Omni3D 1.0
import OmniDebug 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 800
    title: "OmniUI Kitchen Sink"

    Profiler {
        anchors.right: parent.right
        anchors.top: parent.top
        width: 150; height: 80
        visible: true
    }

    // Debug Console Overlay
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width; height: 150
        color: "#AA000000"
        visible: consoleBtn.checked

        ListView {
            anchors.fill: parent
            model: Console.logs
            delegate: Text { text: modelData; color: "white" }
        }
    }

    Button {
        id: consoleBtn
        text: "Console"
        checkable: true
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        z: 100
    }

    TabBar {
        id: bar
        width: parent.width
        TabButton { text: "Widgets" }
        TabButton { text: "Audio & Nodes" }
        TabButton { text: "3D & Vis" }
    }

    StackLayout {
        anchors.top: bar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        currentIndex: bar.currentIndex

        // Tab 1: Widgets
        ScrollView {
            ColumnLayout {
                spacing: 20
                Button { text: "Standard Button" }
                Dial { value: 50 }
                CodeEditor {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200
                    text: "#include <iostream>\nint main() { return 0; }"
                }
                Calendar { }
            }
        }

        // Tab 2: Audio & Nodes
        RowLayout {
            NodeCanvas {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Node { title: "Oscillator 1"; x: 50; y: 50 }
                Node { title: "Filter"; x: 300; y: 100 }
                ConnectionLine { start: Qt.point(150, 100); end: Qt.point(300, 150) }
            }
            ColumnLayout {
                Layout.preferredWidth: 200
                Text { text: "Master Out" }
                Meter {
                    Layout.fillWidth: true
                    height: 20
                    level: 0.8
                }
            }
        }

        // Tab 3: 3D & Vis
        RowLayout {
            View3D {
                Layout.fillWidth: true
                Layout.fillHeight: true
                backgroundColor: "darkblue"
                Mesh { source: "cube.obj" }
                Light { brightness: 2.0 }
            }
            Oscilloscope {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                running: true
            }
        }
    }
}
