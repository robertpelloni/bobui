import QtQuick 2.15
import OmniUI 1.0
import OmniMobile 1.0
import OmniAI 1.0

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: "Mobile Features"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "Sensors"; font.bold: true }
        Switch {
            text: "Active"
            checked: Sensors.active
            onCheckedChanged: Sensors.active = checked
        }

        Button {
            text: "Vibrate"
            onClicked: Haptics.vibrate(200)
        }

        Text { text: "Voice AI"; font.bold: true }
        Button {
            text: "Listen"
            onClicked: Voice.listen()
        }
        Text { id: speechOut; text: "..." }

        Connections {
            target: Voice
            function onSpeechRecognized(text) {
                speechOut.text = text;
                Voice.speak("You said: " + text);
            }
        }
    }
}
