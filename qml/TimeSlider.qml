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
    property int currentTime: 0
    property int totalTime: 0

    property bool isLoaded: false

    signal currentTimeUpdated

    onCurrentTimeChanged: {
        if (!isLoaded)
            return
        if (currentTime < 0)
            currentTime = 0
        if (currentTime > totalTime)
            currentTime = totalTime
        updatePosition()
    }

    function updatePosition() {
        volumePositioner.width = currentTime/totalTime*(timeSlider.width-2)
    }

    Component.onCompleted: isLoaded = true

    id: timeSlider
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
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: timeSlider.width-2
            color: "#7777ee"
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
        id: currentTimeTextItem
        text: core.humanizedTime(currentTime)
        font.pixelSize: (parent.height-22)/2
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.topMargin: 15
        anchors.bottomMargin: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.verticalCenter
        horizontalAlignment: (currentTime<(totalTime/2)) ? "AlignRight" : "AlignLeft"
        color: (currentTime<(totalTime/2)) ? "white" : "black"
    }

    Text {
        id: totalTimeTextItem
        text: core.humanizedTime(totalTime)
        font.pixelSize: (parent.height-22)/2
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.bottomMargin: 15
        anchors.topMargin: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.verticalCenter
        horizontalAlignment: (currentTime<(totalTime/2)) ? "AlignRight" : "AlignLeft"
        color: (currentTime<(totalTime/2)) ? "white" : "black"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            currentTime = totalTime / (timeSlider.width-2) * mouseX
            currentTimeUpdated()
        }
        onPositionChanged: {
            currentTime = totalTime / (timeSlider.width-2) * mouseX
            currentTimeUpdated()
        }
    }
}
