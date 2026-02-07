import QtQuick 2.15
import QtQuick.Layouts 1.15
import OmniUI 1.0
import OmniAudio 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "OmniUI: Advanced Demo"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        anchors.margins: 20

        Text {
            text: "OmniUI Control Panel"
            font.pixelSize: 24
        }

        RowLayout {
            Button {
                text: "Play Note"
                onClicked: midi.sendNoteOn(1, 60, 100)
            }

            Button {
                text: "Stop Note"
                onClicked: midi.sendNoteOff(1, 60)
            }
        }

        Slider {
            Layout.fillWidth: true
            value: 50
            onValueChanged: console.log("Volume: " + value)
        }

        TextField {
            Layout.fillWidth: true
            text: "Hello from QML!"
        }

        // JUCE Visualizer
        JuceView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    MidiHandler {
        id: midi
    }

    AudioProcessor {
        id: dsp
    }
}
