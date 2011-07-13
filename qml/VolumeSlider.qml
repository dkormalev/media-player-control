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

import QtQuick 1.0

Rectangle {
    property int level: 0

    property bool isLoaded: false

    signal levelUpdated

    onLevelChanged: {
        if (!isLoaded)
            return
        if (level < 0)
            level = 0
        if (level > 100)
            level = 100
        updatePosition()
    }

    function updatePosition() {
        volumePositioner.width = level/100*(volumeSlider.width-2)
    }

    Component.onCompleted: isLoaded = true

    id: volumeSlider
    height: 64
    color: "black"
    border.color: "white"
    border.width: 2

    Item {
        id: volumePositioner
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 1
        width: 0
        clip: true

        Rectangle {
            x: 0
            y: width
            height: volumeSlider.width-2
            width: volumeSlider.height
            rotation: -90
            transformOrigin: "TopLeft"
            smooth: true
            gradient: Gradient {
                GradientStop { position: 0.00; color: "#04ee04"; }
                GradientStop { position: 0.50; color: "#f7ff00"; }
                GradientStop { position: 1.00; color: "#ee0404"; }
            }
        }

        Rectangle {
            anchors.fill: parent
            smooth: true
            gradient: Gradient {
                GradientStop {position: 0.00; color: "#50000000"}
                GradientStop {position: 0.20; color: "#15000000"}
                GradientStop {position: 0.50; color: "#00000000"}
                GradientStop {position: 0.80; color: "#15000000"}
                GradientStop {position: 1.00; color: "#50000000"}
            }
        }
    }

    Text {
        id: volumeLevel
        text: level+"%"
        font.pixelSize: 30
        anchors.margins: 15
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: (level<50) ? "AlignRight" : "AlignLeft"
        color: (level<50) ? "white" : "black"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            level = 100 / (volumeSlider.width-2) * mouseX
            levelUpdated()
        }
        onPositionChanged: {
            level = 100 / (volumeSlider.width-2) * mouseX
            levelUpdated()
        }
    }
}
