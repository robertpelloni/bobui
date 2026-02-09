import QtQuick 2.15
import OmniUI 1.0
import OmniCloud 1.0

ApplicationWindow {
    visible: true
    width: 400
    height: 600
    title: "OmniCloud App"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "Cloud Auth"; font.bold: true }

        TextField { id: user; placeholderText: "Username" }
        TextField { id: pass; placeholderText: "Password"; echoMode: TextInput.Password }

        Button {
            text: Auth.authenticated ? "Logout" : "Login"
            onClicked: {
                if (Auth.authenticated) Auth.logout();
                else Auth.login(user.text, pass.text);
            }
        }

        Text { text: "Status: " + (Auth.authenticated ? "Logged in as " + Auth.user : "Guest") }

        Text { text: "Storage"; font.bold: true }

        Button {
            text: "Upload File"
            enabled: Auth.authenticated
            onClicked: Storage.upload("/tmp/test.txt")
        }
    }
}
