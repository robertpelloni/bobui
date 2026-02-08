import QtQuick 2.15
import OmniUI 1.0
import OmniScript 1.0
import OmniNet 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Scripting & Networking"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "Live Scripting"; font.bold: true }

        TextArea {
            id: scriptInput
            width: 400
            height: 100
            text: "console.log('Hello from Script!'); return 42;"
        }

        Button {
            text: "Run"
            onClicked: {
                var result = ScriptEngine.evaluate(scriptInput.text)
                output.text = "Result: " + result
            }
        }

        Text { id: output; text: "Result: " }

        Text { text: "GraphQL Query"; font.bold: true }

        GraphQLClient {
            id: gql
            endpoint: "https://api.spacex.land/graphql/"
            onDataReceived: gqlOutput.text = JSON.stringify(data, null, 2)
        }

        Button {
            text: "Fetch Launch Data"
            onClicked: gql.query("{ launchesPast(limit: 1) { mission_name } }")
        }

        Text { id: gqlOutput; width: 400; wrapMode: Text.WordWrap }
    }
}
