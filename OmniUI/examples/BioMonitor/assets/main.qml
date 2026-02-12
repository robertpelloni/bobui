import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import OmniUI

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "OmniUI Application"

    ColumnLayout {
        anchors.centerIn: parent

        Label {
            text: "Hello from OmniUI!"
            font.pixelSize: 24
        }

        OmniButton {
            text: "Click Me"
            onClicked: console.log("Button clicked!")
        }
    }
}
