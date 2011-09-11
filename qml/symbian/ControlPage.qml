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
import com.nokia.symbian 1.0
import MediaPlayerControl 1.0

Page {
    property QtObject player: core.playerControl()

    property int buttonMargin: 16
    property int buttonWidth: 64

    //HACK: we use full path to predefined image here because of bug while using qml in qrc
    tools: ToolBarLayout {
        ToolButton {
            iconSource: "image://theme/toolbar-home"
            onClicked: core.hideApplication()
        }
        ToolButton {
            iconSource: "image://theme/toolbar-menu"
            onClicked: controlMenu.open()
        }
    }

    Menu {
        id: controlMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {text: "Settings"; onClicked: { pageStack.push(settingsPage) } }
            MenuItem {text: "Fullscreen Player"; onClicked: { player.fullscreen() }}
            MenuItem {text: "Close Player"; onClicked: { player.closePlayer() }}
            MenuItem {text: "Quit"; onClicked: Qt.quit() }
        }
    }

    Rectangle {

        color: "black"
        anchors.fill: parent

        onWidthChanged: {
            volumeSlider.updatePosition()
            timeSlider.updatePosition()
            titleTextItem.text = player.fileName
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
            height: (inPortrait) ? 70 : 32
            anchors.margins: 10
            color: "white"
            font.pixelSize: 24
            wrapMode: "Wrap"
            verticalAlignment: "AlignVCenter"
            onTextChanged: elideText()
            onHeightChanged: {
                titleTextItem.text = player.fileName
                elideText()
            }

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

        Item {
            id: firstLineContainer
            anchors.bottomMargin: buttonMargin
            anchors.leftMargin: buttonMargin

            width: buttonWidth*4 + buttonMargin*3
            height: playButton.height

            PlayerButton {
                id: playButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                source: "qrc:/images/play.svg"
                onClicked: {
                    if (player.state == AbstractPlayerControl.Playing)
                        player.pause()
                    else
                        player.play()
                }
            }


            PlayerButton {
                id: stopButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: playButton.right
                anchors.leftMargin: buttonMargin
                source: "qrc:/images/stop.svg"
                onClicked: player.stop()
            }

            PlayerButton {
                id: prevButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: stopButton.right
                anchors.leftMargin: buttonMargin
                source: "qrc:/images/prev.svg"
                onClicked: player.prev()
            }

            PlayerButton {
                id: nextButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: prevButton.right
                anchors.leftMargin: buttonMargin
                source: "qrc:/images/next.svg"
                onClicked: player.next()
            }

        }

        Item {
            id: secondLineContainer
            anchors.bottomMargin: buttonMargin
            anchors.rightMargin: buttonMargin
            width: buttonWidth*2 + buttonMargin
            height: cycleAudioStreamButton.height


            PlayerButton {
                id: cycleAudioStreamButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                source: "qrc:/images/audio-stream.svg"
                onClicked: player.nextAudioStream()
            }


            PlayerButton {
                id: cycleSubtitleStreamButton
                sourceSize.width: buttonWidth
                anchors.bottom: parent.bottom
                anchors.left: cycleAudioStreamButton.right
                anchors.leftMargin: buttonMargin
                source: "qrc:/images/subtitle-stream.svg"
                onClicked: player.nextSubtitleStream()
            }
        }


        states: [
            State {
                name: "portraitMode"
                when: appWindow.inPortrait
                AnchorChanges {
                    target: firstLineContainer
                    anchors.bottom: secondLineContainer.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.left: undefined
                }
                AnchorChanges {
                    target: secondLineContainer
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.right: undefined
                }
            },
            State {
                name: "landscapeMode"
                when: !appWindow.inPortrait
                AnchorChanges {
                    target: firstLineContainer
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: undefined
                    anchors.left: parent.left
                }
                AnchorChanges {
                    target: secondLineContainer
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: undefined
                    anchors.right: parent.right
                }
            }


        ]



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
                font.pixelSize: 24
            }
            MouseArea{
                anchors.fill: parent
                enabled: fader.visible
            }

        }
    }

}
