import QtQuick 1.1
Rectangle {
    width: 300
    height: 400
    Text {
        id: phoneLabel
        anchors.top: parent.top
        text: "Enter Phone number"
    }
    TextInput {
        id: phoneEdit
        anchors.top: phoneLabel.bottom
        objectName: "phoneEdit"
        width: parent.width
        height: 50
        activeFocusOnPress: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                phoneEdit.forceActiveFocus();
                phoneEdit.openSoftwareInputPanel();
            }
        }
    }

    Text {
        id: messageLabel
        anchors.top: phoneEdit.bottom
        text: "Enter message"
    }
    TextInput {
        id: messageEdit
        anchors.top: messageLabel.bottom
        objectName: "messageEdit"
        width: parent.width
        height: 50
        activeFocusOnPress: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                messageEdit.forceActiveFocus();
                messageEdit.openSoftwareInputPanel();
            }
        }
    }

    Rectangle {
        id: sendButton
        anchors.top: messageEdit.bottom
        width: parent.width
        height: 50
        color: "red"
        border.color: "black"
        border.width: 5
        radius: 10

        Text {
            anchors.fill: parent
            anchors.centerIn: parent.Center
            objectName: "sendButton"
            text: "Send"
            width: parent.width
            height: 50

            MouseArea {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: window.sendSMS()
            }
        }
     }
}
