import QtQuick 1.1
import com.meego 1.0
import MediaPlayerControl 1.0

Page {
    property QtObject player: core.playerControl()

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked:  {
                pageStack.pop()
                player.setNetworkParams(hostField.text, (portField.text)*1)
            }
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent

        Label {
            id: hostLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            text: "Host:"
            color: "white"
        }
        TextField {
            id: hostField
            text: player.host()
            inputMethodHints: Qt.ImhPreferNumbers
            placeholderText: "Enter your computer's IP here"
            anchors.top: hostLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
        }
        Label {
            id: portLabel
            anchors.top: hostField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            text: "Port:"
            color: "white"
        }
        TextField {
            id: portField
            text: (player.port()) ? player.port() : ""
            inputMethodHints: Qt.ImhDigitsOnly
            placeholderText: "Enter your computer's port here"
            anchors.top: portLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
        }
    }


}
