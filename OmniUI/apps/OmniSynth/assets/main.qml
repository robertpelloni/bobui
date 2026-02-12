import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniAudio 1.0
import OmniNodes 1.0

import OmniCharts 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 800
    title: "OmniSynth"

    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical

        // Top: Node Editor & Visualizers
        SplitView {
            SplitView.fillHeight: true
            orientation: Qt.Horizontal

            // Node Graph
            NodeCanvas {
                SplitView.preferredWidth: 800
                SplitView.fillHeight: true

                Node { title: "Wavetable OSC"; x: 50; y: 50 }
                Node { title: "LowPass Filter"; x: 300; y: 100 }
                Node { title: "Output"; x: 550; y: 150 }

                ConnectionLine { start: Qt.point(200, 100); end: Qt.point(300, 150) }
                ConnectionLine { start: Qt.point(450, 150); end: Qt.point(550, 200) }
            }

            // Visualizers
            ColumnLayout {
                SplitView.fillWidth: true

                Oscilloscope {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    running: true
                    color: "cyan"
                }

                Spectrogram {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    running: true
                }
            }
        }

        // Bottom: Controls
        RowLayout {
            height: 200
            Layout.margins: 20
            spacing: 40

            ColumnLayout {
                Text { text: "Oscillator 1"; font.bold: true; color: "white" }
                Dial { value: 50 }
                Text { text: "Shape" }
            }

            ColumnLayout {
                Text { text: "Filter"; font.bold: true; color: "white" }
                Dial { value: 75 }
                Text { text: "Cutoff" }
            }

            ColumnLayout {
                Text { text: "Master"; font.bold: true; color: "white" }
                Slider { value: 80 }
                Meter {
                    Layout.fillWidth: true
                    height: 20
                    level: 0.8
                }
            }
        }
    }
}
