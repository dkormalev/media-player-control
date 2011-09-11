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

#ifndef VLCPLAYERCONTROL_H
#define VLCPLAYERCONTROL_H

#include "abstractplayercontrol.h"

class VlcPlayerControlPrivate;
class QThread;

class VlcPlayerControl : public AbstractPlayerControl
{
    Q_OBJECT
public:
    explicit VlcPlayerControl(QObject *parent = 0);
    ~VlcPlayerControl();

    Q_INVOKABLE QString playerName() const;

    qulonglong totalTime() const;
    qulonglong currentTime() const;
    int volume() const;
    QString fileName() const;
    AbstractPlayerControl::PlayerState state() const;
    Q_INVOKABLE QString host() const;
    Q_INVOKABLE int port() const;

public slots:
    void play();
    void pause();
    void stop();
    void next();
    void prev();
    void closePlayer();
    void fullscreen();
    void nextAudioStream();
    void nextSubtitleStream();
    void seek(qulonglong time);
    void changeVolume(int volume);
    void setNetworkParams(const QString &host, int port);

protected:
    void initPlayer();
    void deInitPlayer();

private:
    QThread *thread;
    VlcPlayerControlPrivate *d_ptr;
    Q_DECLARE_PRIVATE(VlcPlayerControl)
};

#endif // VLCPLAYERCONTROL_H
