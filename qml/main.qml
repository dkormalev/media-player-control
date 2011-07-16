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
import MediaPlayerControl 1.0

Rectangle {
    property QtObject player: core.playerControl()
    color: "black"

    onWidthChanged: {
        volumeSlider.updatePosition()
        timeSlider.updatePosition()
        titleTextItem.text = player.fileName
        titleTextItem.elideText()
    }

    Connections {
        target: player
        onVolumeChanged: volumeSlider.level = player.volume
        onTotalTimeChanged: timeSlider.totalTime = player.totalTime
        onCurrentTimeChanged: timeSlider.currentTime = player.currentTime
        onStateChanged: playButton.source = (player.state == AbstractPlayerControl.Playing) ? "qrc:/images/pause.svg" : "qrc:/images/play.svg"
        onFileNameChanged: titleTextItem.text = player.fileName
    }

    Component.onCompleted: {
        volumeSlider.level = player.volume
        timeSlider.totalTime = player.totalTime
        timeSlider.currentTime = player.currentTime
        titleTextItem.text = player.fileName
        playButton.source = (player.state == AbstractPlayerControl.Playing) ? "qrc:/images/pause.svg" : "qrc:/images/play.svg"
    }

    Text {
        //NOTE: Maybe someday it will be possible to use both elide and wrapMode for Text element? For now it is impossible, so we are doing it manually
        function elideText() {
            var lastHeight = titleTextItem.paintedHeight
            while (titleTextItem.height < titleTextItem.paintedHeight &&
                   titleTextItem.text.length > 5) {
                var visibleZone = Math.floor(titleTextItem.text.length /
                                             (titleTextItem.paintedHeight /
                                              titleTextItem.height))
                console.log(titleTextItem.text.length+"; "+titleTextItem.paintedHeight+"; "+titleTextItem.height+"; "+visibleZone)
                titleTextItem.text = titleTextItem.text.substring(0, visibleZone-2) + "…"
                if (lastHeight == titleTextItem.paintedHeight)
                    return
            }
        }

        id: titleTextItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 50
        anchors.margins: 10
        color: "white"
        font.pixelSize: 20
        wrapMode: "Wrap"
        verticalAlignment: "AlignVCenter"

        onTextChanged: elideText()

    }

    VolumeSlider {
        id: volumeSlider
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleTextItem.bottom
        anchors.margins: 10
        onLevelUpdated: player.changeVolume(level)
    }

    TimeSlider {
        id: timeSlider
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: volumeSlider.bottom
        anchors.margins: 10
        onCurrentTimeUpdated: player.seek(currentTime)
    }

    Button {
        id: playButton
        anchors.bottom: closeButton.top
        anchors.left: parent.left
        anchors.margins: 15
        source: "qrc:/images/play.svg"
        onClicked: {
            if (player.state == AbstractPlayerControl.Playing)
                player.pause()
            else
                player.play()
        }
    }


    Button {
        id: stopButton
        anchors.bottom: closeButton.top
        anchors.left: playButton.right
        anchors.margins: 15
        source: "qrc:/images/stop.svg"
        onClicked: player.stop()
    }

    Button {
        id: prevButton
        anchors.bottom: closeButton.top
        anchors.left: stopButton.right
        anchors.margins: 15
        source: "qrc:/images/prev.svg"
        onClicked: player.prev()
    }

    Button {
        id: nextButton
        anchors.bottom: closeButton.top
        anchors.left: prevButton.right
        anchors.margins: 15
        source: "qrc:/images/next.svg"
        onClicked: player.next()
    }

    Button {
        id: fullscreenButton
        anchors.bottom: parent.bottom
        anchors.right: closeButton.left
        anchors.margins: 15
        source: "qrc:/images/fullscreen.svg"
        onClicked: player.fullscreen()
    }

    Button {
        id: closeButton
        anchors.bottom: parent.bottom
        anchors.left: nextButton.left
        anchors.bottomMargin: 15
        source: "qrc:/images/close.svg"
        onClicked: player.closePlayer()
    }

    Item {
        id: fader
        anchors.fill: parent
        visible: player.state == AbstractPlayerControl.NotConnected
        Rectangle {
            anchors.fill: parent
            opacity: fader.visible ? 0.5 : 0.0
            color: "black"
        }
        Text {
            anchors.fill: parent
            horizontalAlignment: "AlignHCenter"
            verticalAlignment: "AlignVCenter"
            color: "white"
            text: "Not Connected"
            font.pixelSize: 20
        }
        MouseArea{
            anchors.fill: parent
            enabled: fader.visible
        }

    }
}
