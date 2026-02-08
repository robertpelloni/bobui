import QtQuick 2.15
import OmniUI 1.0
import OmniNodes 1.0
import OmniAI 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Visual Builder & AI"

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            TextField { id: prompt; placeholderText: "Ask OmniAI..." }
            Button {
                text: "Send"
                onClicked: ai.chat(prompt.text)
            }
            Text { text: statusText; id: status }
        }

        LLMClient {
            id: ai
            onResponseReceived: status.text = response
        }

        NodeCanvas {
            Layout.fillWidth: true
            Layout.fillHeight: true
            zoom: 1.0

            // Example Nodes
            Node {
                x: 100; y: 100
                width: 150; height: 100
                title: "Oscillator"
                Rectangle { anchors.fill: parent; color: "#333"; opacity: 0.5 }
            }

            Node {
                x: 400; y: 200
                width: 150; height: 100
                title: "Filter"
                Rectangle { anchors.fill: parent; color: "#333"; opacity: 0.5 }
            }

            ConnectionLine {
                start: Qt.point(250, 150)
                end: Qt.point(400, 250)
                color: "cyan"
            }
        }
    }
}
