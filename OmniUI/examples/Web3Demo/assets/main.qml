import QtQuick 2.15
import OmniUI 1.0
import OmniWeb3 1.0

ApplicationWindow {
    visible: true
    width: 400
    height: 500
    title: "Web3 Wallet"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text { text: "Crypto Wallet"; font.bold: true }

        Text { text: "Address: " + Wallet.address }
        Text { text: "Balance: " + Wallet.balance + " ETH" }

        Button {
            text: "Connect MetaMask"
            onClicked: Wallet.connect("metamask")
        }

        SmartContract {
            id: contract
            address: "0xContractAddress"
            onResultReceived: console.log("Contract Result:", result)
        }

        Button {
            text: "Call Contract"
            onClicked: contract.call("mint", [])
        }
    }
}
