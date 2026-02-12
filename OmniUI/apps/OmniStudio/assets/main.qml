import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniWidgets 1.0
import OmniNodes 1.0
import Omni3D 1.0
import OmniDebug 1.0
import OmniSystem 1.0

ApplicationWindow {
    visible: true
    width: 1600
    height: 900
    title: "OmniStudio " + System.osName

    MenuBar {
        Menu {
            title: "File"
            Action { text: "New Project" }
            Action { text: "Open..." }
            Action { text: "Save" }
            Separator {}
            Action { text: "Exit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "View"
            Action { text: "Toggle Console"; onTriggered: consoleDock.visible = !consoleDock.visible }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Sidebar
        ColumnLayout {
            SplitView.preferredWidth: 250
            SplitView.minimumWidth: 200

            TabBar {
                Layout.fillWidth: true
                TabButton { text: "Project" }
                TabButton { text: "Assets" }
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ["main.qml", "AudioGraph.qml", "Scene3D.qml", "Logic.js"]
                delegate: ItemDelegate { text: modelData; width: parent.width }
            }
        }

        // Main Editor Area
        SplitView {
            Layout.fillWidth: true
            orientation: Qt.Vertical

            // Top: Editors
            SplitView {
                SplitView.fillHeight: true
                orientation: Qt.Horizontal

                // Code
                CodeEditor {
                    SplitView.preferredWidth: 600
                    SplitView.fillHeight: true
                    text: "import QtQuick 2.0\n\nItem {\n    // OmniUI Code\n}"
                }

                // Visual Graph
                NodeCanvas {
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    Node { title: "Audio Input"; x: 50; y: 50 }
                    Node { title: "Mixer"; x: 300; y: 100 }
                }
            }

            // Bottom: Console/Preview
            SplitView {
                id: consoleDock
                SplitView.preferredHeight: 200
                orientation: Qt.Horizontal

                // Console
                ListView {
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    model: Console.logs
                    delegate: Text { text: modelData; font.family: "Monospace" }
                }

                // 3D Preview
                View3D {
                    SplitView.preferredWidth: 300
                    SplitView.fillHeight: true
                    backgroundColor: "#222"
                    Mesh { source: "preview.obj" }
                }
            }
        }

        // Inspector
        ColumnLayout {
            SplitView.preferredWidth: 300
            Text { text: "Inspector"; font.bold: true; Layout.margins: 10 }

            // Mock Inspector UI
            RowLayout {
                Text { text: "Position X" }
                SpinBox { value: 100 }
            }
            RowLayout {
                Text { text: "Position Y" }
                SpinBox { value: 50 }
            }

            Item { Layout.fillHeight: true }
        }
    }

    // Status Bar
    footer: ToolBar {
        RowLayout {
            Text { text: "Ready" }
            Item { Layout.fillWidth: true }
            Text { text: "Battery: " + System.batteryLevel + "%" }
        }
    }
}
