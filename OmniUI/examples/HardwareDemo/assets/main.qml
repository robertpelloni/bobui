import QtQuick 2.15
import OmniUI 1.0
import OmniHardware 1.0
import OmniDesktop 1.0

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "Hardware & Desktop Demo"

    SystemTray {
        visible: true
        icon: "qrc:/icon.png"
        toolTip: "OmniUI Demo"
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text { text: "Serial Ports:"; font.bold: true }

        SerialPort {
            id: serial
            onDataReceived: console.log("RX:", data)
        }

        ComboBox {
            model: serial.availablePorts()
            onCurrentTextChanged: serial.portName = currentText
        }

        Button {
            text: serial.open ? "Close" : "Open"
            onClicked: serial.open = !serial.open
        }

        Text { text: "Bluetooth Scan:"; font.bold: true }

        Bluetooth {
            id: ble
            onDeviceDiscovered: console.log("Found device:", name, address)
        }

        Button {
            text: ble.scanning ? "Scanning..." : "Start Scan"
            onClicked: ble.startDiscovery()
        }
    }
}
