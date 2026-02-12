import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniMobile 1.0
import OmniCharts 1.0
import OmniCloud 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 600
    title: "OmniDash IoT"

    GridLayout {
        anchors.fill: parent
        columns: 3

        // Sensor Data
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#222"
            ColumnLayout {
                anchors.centerIn: parent
                Text { text: "Accelerometer"; color: "white" }
                LineChart {
                    Layout.preferredWidth: 200; Layout.preferredHeight: 100
                    data: [0.1, 0.5, 0.2, 0.8, 0.3]
                    color: "red"
                }
            }
        }

        // Location
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#222"
            ColumnLayout {
                anchors.centerIn: parent
                Text { text: "GPS Tracking"; color: "white" }
                Text { text: "Lat: " + Location.latitude; color: "cyan" }
                Text { text: "Lon: " + Location.longitude; color: "cyan" }
            }
        }

        // Cloud Control
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: "#222"
            ColumnLayout {
                anchors.centerIn: parent
                Text { text: "MQTT Control"; color: "white" }
                Dial {
                    id: tempDial
                    value: 22
                    onValueChanged: PubSub.publish("home/livingroom/temp", value)
                }
                Text { text: "Set Temp: " + tempDial.value + "°C"; color: "white" }
            }
        }
    }

    Connections {
        target: PubSub
        function onMessageReceived(topic, message) {
            console.log("MQTT:", topic, message)
        }
    }

    Component.onCompleted: {
        PubSub.subscribe("home/#")
    }
}
