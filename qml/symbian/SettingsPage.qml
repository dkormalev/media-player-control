/**************************************************************************
**
** This file is part of Media Player Control
**
** Copyright (C) 2011  Denis Kormalev <kormalev.denis@gmail.com>
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** This application is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
**
**************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.0
import MediaPlayerControl 1.0

Page {
    property QtObject player: core.playerControl()

    //HACK: we use full path to predefined image here because of bug while using qml in qrc
    tools: ToolBarLayout {
        ToolButton {
            iconSource: "image://theme/toolbar-back"
            onClicked:  {
                player.setNetworkParams(hostField.text, (portField.text)*1)
                pageStack.pop()
            }
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent

        Text {
            id: hostLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            text: "Host:"
            font.pixelSize: 20
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
        Text {
            id: portLabel
            anchors.top: hostField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            text: "Port:"
            font.pixelSize: 20
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
