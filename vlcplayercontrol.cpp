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

#include "vlcplayercontrol.h"
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QTimerEvent>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QThread>
#include <QDateTime>

class VlcPlayerControlPrivate : public QObject
{
    Q_OBJECT
private:
    VlcPlayerControlPrivate(VlcPlayerControl *control);
    VlcPlayerControl *q_ptr;
    Q_DECLARE_PUBLIC(VlcPlayerControl)

    QTcpSocket *socket;
    QString hostName;
    unsigned port;
    qulonglong totalTime;
    qulonglong currentTime;
    int volume;
    QString fileName;
    AbstractPlayerControl::PlayerState state;
    int updateStatusTimerId;
    QDateTime lastTimePauseCheck;
    bool manualSeek;

    bool inNetworkRequest;


    QString runCommand(const QString &command);
    void updateStatus();
    QString waitForPrompt();

    bool socketStillAlive();

    void sendCommand(const QString &command, bool requestUpdate);

public slots:
    void init();
    void runner(const QString &command, bool requestUpdate);
    bool connectToServer();

signals:
    void totalTimeChanged(qulonglong value);
    void currentTimeChanged(qulonglong value);
    void volumeChanged(int value);
    void fileNameChanged(const QString &value);
    void stateChanged(AbstractPlayerControl::PlayerState value);

protected:
    void timerEvent(QTimerEvent *ev);

};


VlcPlayerControlPrivate::VlcPlayerControlPrivate(VlcPlayerControl *control)
    :QObject(0), q_ptr(control), hostName("192.168.1.2"), port(14212),
      totalTime(0), currentTime(0), volume(0), state(AbstractPlayerControl::NotConnected),
      lastTimePauseCheck(QDateTime::currentDateTime()), manualSeek(false), inNetworkRequest(false)
{
}

void VlcPlayerControlPrivate::init()
{
    socket = new QTcpSocket(this);
    updateStatusTimerId = startTimer(500);
    if (connectToServer())
        updateStatus();
}

bool VlcPlayerControlPrivate::connectToServer()
{
    qDebug() << Q_FUNC_INFO << hostName << port;
    socket->connectToHost(hostName, port);
    socket->waitForConnected(1000);
    if (socket->state() == QAbstractSocket::ConnectedState) {
        state = AbstractPlayerControl::Stopped;
        waitForPrompt();
        emit stateChanged(state);
        qDebug() << Q_FUNC_INFO << "connected";
        return state != AbstractPlayerControl::NotConnected;
    } else {
        QTimer::singleShot(500, this, SLOT(connectToServer()));
        return false;
    }
}


bool VlcPlayerControlPrivate::socketStillAlive()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << Q_FUNC_INFO << "disconnected";
        state = AbstractPlayerControl::NotConnected;
        emit stateChanged(state);
        QTimer::singleShot(0, this, SLOT(connectToServer()));
        return false;
    }
    return true;
}


QString VlcPlayerControlPrivate::runCommand(const QString &command)
{
    QByteArray toSend;
    toSend.append(command);
    toSend.append("\n");
    socket->write(toSend);
    socket->waitForBytesWritten(1000);
    if (socketStillAlive())
        return waitForPrompt();
    else
        return "";
}

void VlcPlayerControlPrivate::runner(const QString &command, bool requestUpdate)
{
    while (inNetworkRequest) {
        QApplication::processEvents();
        QThread::yieldCurrentThread();
    }
    inNetworkRequest = true;
    runCommand(command);
    if (requestUpdate)
        updateStatus();
    inNetworkRequest = false;
}

void VlcPlayerControlPrivate::sendCommand(const QString &command, bool requestUpdate)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;
    QMetaObject::invokeMethod(this, "runner",
                              Qt::QueuedConnection,
                              Q_ARG(QString, command),
                              Q_ARG(bool, requestUpdate));

}

void VlcPlayerControlPrivate::updateStatus()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        if (state != AbstractPlayerControl::NotConnected)
            socketStillAlive();
        return;
    }
    AbstractPlayerControl::PlayerState newState = AbstractPlayerControl::NotConnected;
    QString rawStatus = runCommand("status");
    QStringList statusStrings = rawStatus.split("\n");
    QString volString;
    QString stateString;
    if (statusStrings.size() >= 3) {
        volString = statusStrings.at(1);
        stateString = statusStrings.at(2);
    } else if (statusStrings.size() == 2) {
        volString = statusStrings.at(0);
        stateString = statusStrings.at(1);
    }
    if (statusStrings.size() >= 2) {
        int volValue = 0;
        volString.remove("( audio volume:");
        volString.remove(")");
        volValue = volString.trimmed().toInt();
        volValue = qRound((double)volValue/5.12);
        if (volValue != volume) {
            volume = volValue;
            emit volumeChanged(volume);
        }

        stateString.remove("( state");
        stateString.remove(")");
        stateString = stateString.trimmed().toLower();
        if (stateString == "stopped") {
            newState = AbstractPlayerControl::Stopped;
        } else if (stateString == "paused") {
            newState = AbstractPlayerControl::Paused;
        } else if (stateString == "playing") {
            newState = AbstractPlayerControl::Playing;
        }
    }
    QString rawTotalTime = runCommand("get_length");
    bool ok;
    qulonglong newTotalTime = rawTotalTime.toULongLong(&ok);
    if (ok) {
        if (newTotalTime != totalTime) {
            totalTime = newTotalTime;
            emit totalTimeChanged(totalTime);
        }
    }

    QString rawCurrentTime = runCommand("get_time");
    qulonglong newCurrentTime = rawCurrentTime.toULongLong(&ok);
    if (ok) {
        qDebug() << Q_FUNC_INFO << newCurrentTime << currentTime << state << newState << lastTimePauseCheck.msecsTo(QDateTime::currentDateTime());
        if (newCurrentTime == currentTime && lastTimePauseCheck.msecsTo(QDateTime::currentDateTime()) > 1100) {
            if (state == AbstractPlayerControl::Playing ||
                (newState == AbstractPlayerControl::Playing && state == AbstractPlayerControl::Paused)) {
                newState = AbstractPlayerControl::Paused;
            }
        } else {
            if (currentTime != newCurrentTime)
                lastTimePauseCheck = QDateTime::currentDateTime();
            currentTime = newCurrentTime;
            emit currentTimeChanged(currentTime);
            if (manualSeek)
                manualSeek = false;
            else
                newState = AbstractPlayerControl::Playing;
        }
    } else {
        newState = AbstractPlayerControl::Stopped;
    }

    QString rawTitle = runCommand("get_title");
    if (rawTitle != fileName) {
        fileName = rawTitle;
        emit fileNameChanged(fileName);
    }


    if (newState != state) {
        state = newState;
        emit stateChanged(state);
    }
}

void VlcPlayerControlPrivate::timerEvent(QTimerEvent *ev)
{
    if (updateStatusTimerId == ev->timerId()) {
        if (inNetworkRequest)
            return;
        inNetworkRequest = true;
        updateStatus();
        inNetworkRequest = false;
    }
}

QString VlcPlayerControlPrivate::waitForPrompt()
{
    QString result;
    bool returnValueHere = false;
    while(!returnValueHere) {
        socket->waitForReadyRead(1000);
        if (!socketStillAlive())
            return "";
        QByteArray data = socket->readAll();
        if (data.endsWith("> ")) {
            data.remove(data.size()-2, 2);
            returnValueHere = true;
        }
        result += data;
    }
    return result.trimmed();
}

VlcPlayerControl::VlcPlayerControl(QObject *parent) :
    AbstractPlayerControl(parent), d_ptr(new VlcPlayerControlPrivate(this))
{
    Q_D(VlcPlayerControl);
    connect(d, SIGNAL(currentTimeChanged(qulonglong)), this, SIGNAL(currentTimeChanged(qulonglong)));
    connect(d, SIGNAL(volumeChanged(int)), this, SIGNAL(volumeChanged(int)));
    connect(d, SIGNAL(fileNameChanged(QString)), this, SIGNAL(fileNameChanged(QString)));
    connect(d, SIGNAL(stateChanged(AbstractPlayerControl::PlayerState)), this, SIGNAL(stateChanged(AbstractPlayerControl::PlayerState)));
    connect(d, SIGNAL(totalTimeChanged(qulonglong)), this, SIGNAL(totalTimeChanged(qulonglong)));
}

VlcPlayerControl::~VlcPlayerControl()
{
    delete d_ptr;
}

QString VlcPlayerControl::playerName() const
{
    return "VLC";
}

qulonglong VlcPlayerControl::totalTime() const
{
    Q_D(const VlcPlayerControl);
    return d->totalTime;
}

qulonglong VlcPlayerControl::currentTime() const
{
    Q_D(const VlcPlayerControl);
    return d->currentTime;
}

int VlcPlayerControl::volume() const
{
    Q_D(const VlcPlayerControl);
    return d->volume;
}

QString VlcPlayerControl::fileName() const
{
    Q_D(const VlcPlayerControl);
    return d->fileName;
}

AbstractPlayerControl::PlayerState VlcPlayerControl::state() const
{
    Q_D(const VlcPlayerControl);
    return d->state;
}

void VlcPlayerControl::play()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("play", true);
}

void VlcPlayerControl::pause()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("pause", true);
}

void VlcPlayerControl::stop()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("stop", true);
}

void VlcPlayerControl::next()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("next", true);
}

void VlcPlayerControl::prev()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("prev", true);
}

void VlcPlayerControl::closePlayer()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("shutdown", false);
}

void VlcPlayerControl::fullscreen()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("fullscreen", false);
}

void VlcPlayerControl::nextAudioStream()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("key audio-track", false);
}

void VlcPlayerControl::nextSubtitleStream()
{
    Q_D(VlcPlayerControl);
    d->sendCommand("key subtitle-track", false);
}

void VlcPlayerControl::seek(qulonglong time)
{
    Q_D(VlcPlayerControl);
    if (time > d->totalTime)
        time = d->totalTime;
    d->manualSeek = true;
    d->sendCommand(QString("seek %1").arg(time), false);
}

void VlcPlayerControl::changeVolume(int volume)
{
    Q_D(VlcPlayerControl);
    if (d->volume == volume)
        return;
    if (volume > 100)
        volume = 100;
    if (volume < 0)
        volume = 0;
    volume *= 5.12;
    d->sendCommand(QString("volume %1").arg(volume), false);
}

QStringList VlcPlayerControl::params() const
{
    QStringList result;
    result << "Host" << "Port";
    return result;
}

void VlcPlayerControl::setParam(const QString &paramName, const QString &value)
{
    Q_D(VlcPlayerControl);
    if (paramName == "Host")
        d->hostName = value;
    else if (paramName == "Port")
        d->port = value.toUInt();
}

QString VlcPlayerControl::param(const QString &paramName) const
{
    Q_D(const VlcPlayerControl);
    if (paramName == "Host")
        return d->hostName;
    else if (paramName == "Port")
        return QString::number(d->port);
    return "";
}

void VlcPlayerControl::initPlayer()
{
    Q_D(VlcPlayerControl);
    thread = new QThread(this);
    thread->start();
    d->moveToThread(thread);
    QMetaObject::invokeMethod(d, "init", Qt::QueuedConnection);
}

void VlcPlayerControl::deInitPlayer()
{
    thread->quit();
    thread->deleteLater();
}



#include "vlcplayercontrol.moc"
