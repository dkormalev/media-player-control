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

#ifndef ABSTRACTPLAYERCONTROL_H
#define ABSTRACTPLAYERCONTROL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMetaType>

class AbstractPlayerControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qulonglong totalTime READ totalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(qulonglong currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(AbstractPlayerControl::PlayerState state READ state NOTIFY stateChanged)
    Q_ENUMS(PlayerState)

public:
    enum PlayerState
    {
        NotConnected,
        Stopped,
        Playing,
        Paused
    };

    explicit AbstractPlayerControl(QObject *parent = 0);

    void init();
    void deInit();

    Q_INVOKABLE virtual QString playerName() const = 0;

    virtual qulonglong totalTime() const = 0;
    virtual qulonglong currentTime() const = 0;
    virtual int volume() const = 0;
    virtual QString fileName() const = 0;
    virtual AbstractPlayerControl::PlayerState state() const = 0;
    Q_INVOKABLE virtual QStringList params() const = 0;
    Q_INVOKABLE virtual QString param(const QString &paramName) const = 0;


public slots:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;
    virtual void closePlayer() = 0;
    virtual void fullscreen() = 0;
    virtual void seek(qulonglong time) = 0;
    virtual void changeVolume(int volume) = 0;
    virtual void setParam(const QString &paramName, const QString &value) = 0;


signals:
    void totalTimeChanged(qulonglong value);
    void currentTimeChanged(qulonglong value);
    void volumeChanged(int value);
    void fileNameChanged(const QString &value);
    void stateChanged(AbstractPlayerControl::PlayerState value);

protected:
    virtual void initPlayer() = 0;
    virtual void deInitPlayer() = 0;

};

Q_DECLARE_METATYPE(AbstractPlayerControl::PlayerState)

#endif // ABSTRACTPLAYERCONTROL_H
