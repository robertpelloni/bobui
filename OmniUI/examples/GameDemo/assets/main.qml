import QtQuick 2.15
import OmniUI 1.0
import OmniGame 1.0
import OmniPhysics 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "OmniGame Engine"

    PhysicsWorld {
        id: world
        anchors.fill: parent
        running: true

        RigidBody {
            id: player
            x: 100; y: 100
            width: 64; height: 64

            Sprite {
                anchors.fill: parent
                source: "player.png"
                frameIndex: Input.isPressed("Jump") ? 1 : 0
            }
        }
    }

    Timer {
        interval: 16; running: true; repeat: true
        onTriggered: {
            if (Input.isPressed("Right")) player.velocity = Qt.vector2d(200, player.velocity.y);
        }
    }

    Component.onCompleted: {
        Input.mapKey(Qt.Key_Right, "Right");
        Input.mapKey(Qt.Key_Space, "Jump");
    }
}
