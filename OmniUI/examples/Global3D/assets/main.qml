import QtQuick 2.15
import OmniUI 1.0
import Omni3D 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: Translator.tr("3D World")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Button {
                text: "English"
                onClicked: Translator.language = "en"
            }
            Button {
                text: "Español"
                onClicked: Translator.language = "es"
            }
            Button {
                text: "Deutsch"
                onClicked: Translator.language = "de"
            }
        }

        View3D {
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "black"

            Light {
                color: "white"
                brightness: 1.5
            }

            Mesh {
                source: "models/cube.obj"
                position: Qt.vector3d(0, 0, -5)
                scale: Qt.vector3d(2, 2, 2)
            }
        }
    }
}
