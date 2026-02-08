import QtQuick 2.15
import OmniUI 1.0
import OmniData 1.0
import OmniGfx 1.0

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "OmniUI: Plugin & Gfx Demo"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "Plugin System"; font.bold: true }

        Button {
            text: "Load Plugins"
            onClicked: PluginManager.loadPlugins("./plugins")
        }

        Text { text: "User Settings"; font.bold: true }

        TextField {
            id: username
            text: Settings.value("username", "Guest")
            onTextChanged: Settings.setValue("username", text)
        }

        Text { text: "Graphics"; font.bold: true }

        ShaderEffect {
            width: 200; height: 100
            fragmentShader: "void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }"
        }
    }
}
