import QtQuick 2.15
import OmniUI 1.0
import OmniCollab 1.0
import OmniAudio 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Collaborative Workspace"

    SyncClient {
        id: client
        userId: "User_" + Math.floor(Math.random() * 1000)
        onConnectedChanged: console.log("Connected:", connected)
        onRemoteCursorMoved: {
            var cursor = cursors[userId] || createCursor(userId);
            cursor.x = pos.x;
            cursor.y = pos.y;
        }
    }

    // Remote cursors container
    Item { id: cursorLayer; anchors.fill: parent }
    property var cursors: ({})

    function createCursor(id) {
        var comp = Qt.createComponent("Cursor.qml");
        var obj = comp.createObject(cursorLayer, {label: id});
        cursors[id] = obj;
        return obj;
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: client.sendCursor(Qt.point(mouseX, mouseY))
    }

    Column {
        anchors.centerIn: parent
        Button {
            text: "Connect"
            onClicked: client.connectToServer("ws://localhost:8080")
        }

        Text { text: "Audio Patchbay"; font.bold: true }

        // Hypothetical PatchBay visualizer
        PatchBay {
            width: 400; height: 300
            // graph: audioGraph
        }
    }
}
