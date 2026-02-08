import QtQuick 2.15
import QtQuick.Layouts 1.15
import OmniUI 1.0
import OmniCharts 1.0
import OmniLayout 1.0
import OmniAudio 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "OmniUI: Complex Workstation"

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Left Panel: Sequencer & Controls
        DockArea {
            width: 300
            areaName: "Controls"

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Text { text: "Sequencer"; font.bold: true }

                Sequencer {
                    id: seq
                    bpm: 140
                }

                Text { text: "Output Level" }
                Meter {
                    Layout.fillWidth: true
                    height: 20
                    level: Math.random() // Mock level
                    running: true
                    Timer {
                        interval: 100; running: true; repeat: true
                        onTriggered: parent.level = Math.random()
                    }
                }

                RowLayout {
                    Button { text: "Start"; onClicked: seq.start() }
                    Button { text: "Stop"; onClicked: seq.stop() }
                }

                Dial {
                    value: seq.bpm
                    onValueChanged: seq.bpm = value
                }
                Text { text: "BPM: " + seq.bpm }
            }
        }

        // Center Panel: Visualizers
        DockArea {
            Layout.fillWidth: true
            areaName: "Visuals"

            ColumnLayout {
                anchors.fill: parent

                Oscilloscope {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "cyan"
                    running: true
                }

                LineChart {
                    Layout.fillWidth: true
                    Layout.height: 200
                    data: [10, 50, 20, 80, 40, 90, 30]
                    color: "magenta"
                }
            }
        }

        // Right Panel: DSP Chain
        DockArea {
            width: 250
            areaName: "DSP"

            ColumnLayout {
                Text { text: "Master Chain"; font.bold: true }

                Gain {
                    id: masterGain
                    gain: 0.8
                }
                Slider {
                    Layout.fillWidth: true
                    value: masterGain.gain * 100
                    onValueChanged: masterGain.gain = value / 100
                }

                Filter {
                    id: lpFilter
                    type: Filter.LowPass
                    cutoff: 2000
                }
                Dial {
                    value: lpFilter.cutoff
                    // Map dial 0-100 to 20-20000 Hz log scale logic here
                }
                Text { text: "Cutoff" }
            }
        }
    }
}
