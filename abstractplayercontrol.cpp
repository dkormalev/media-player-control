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

#include "abstractplayercontrol.h"
#include <QSettings>
#include <QDebug>

AbstractPlayerControl::AbstractPlayerControl(QObject *parent) : QObject(parent)
{
}

void AbstractPlayerControl::init()
{
    QSettings settings;
    settings.beginGroup(playerName());
    setNetworkParams(settings.value("host", "").toString(), settings.value("port", 0).toInt());
    settings.endGroup();
    initPlayer();
}

void AbstractPlayerControl::deInit()
{
    deInitPlayer();
    saveNetworkParams();
}

void AbstractPlayerControl::saveNetworkParams()
{
    QSettings settings;
    settings.beginGroup(playerName());
    settings.setValue("host", host());
    settings.setValue("port", port());
    settings.endGroup();
}

