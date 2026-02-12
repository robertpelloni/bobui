import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniAI 1.0
import OmniWidgets 1.0

ApplicationWindow {
    visible: true
    width: 600
    height: 800
    title: "OmniChat AI"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        ListView {
            id: chatHistory
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ListModel {}
            delegate: Rectangle {
                width: parent.width
                height: msgText.paintedHeight + 20
                color: model.role === "user" ? "#007BFF" : "#333"
                radius: 10

                Text {
                    id: msgText
                    anchors.centerIn: parent
                    width: parent.width - 20
                    text: model.content
                    color: "white"
                    wrapMode: Text.WordWrap
                }
            }
        }

        // Code Block Preview
        CodeEditor {
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            text: "// AI generated code will appear here"
            readOnly: true
        }

        RowLayout {
            Layout.fillWidth: true

            TextField {
                id: inputField
                Layout.fillWidth: true
                placeholderText: "Type a message..."
                onAccepted: sendMessage()
            }

            Button {
                text: "Send"
                onClicked: sendMessage()
            }

            Button {
                text: "🎤"
                onClicked: Voice.listen()
            }
        }
    }

    LLMClient {
        id: llm
        onResponseReceived: {
            chatHistory.model.append({role: "assistant", content: response})
            Voice.speak(response)
        }
    }

    Connections {
        target: Voice
        function onSpeechRecognized(text) {
            inputField.text = text
            sendMessage()
        }
    }

    function sendMessage() {
        if (inputField.text === "") return
        chatHistory.model.append({role: "user", content: inputField.text})
        llm.chat(inputField.text)
        inputField.text = ""
    }
}
