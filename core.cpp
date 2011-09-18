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

#include "core.h"
#include <QtDeclarative>
#include "vlcplayercontrol.h"

#ifdef Q_OS_SYMBIAN
# include <w32std.h>
# include <mw/coemain.h>
#endif

Core *Core::m_instance = 0;

Core::Core(QObject *parent) :
    QObject(parent)
{
    AbstractPlayerControl *vlc = new VlcPlayerControl;
    vlc->init();
    m_playerControls.insert(vlc->playerName(), vlc);
}

Core *Core::instance()
{
    if (!m_instance)
        m_instance = new Core;
    return m_instance;
}

void Core::setDeclarativeView(QDeclarativeView *view)
{
    m_view = view;
    qRegisterMetaType<AbstractPlayerControl::PlayerState>();
    qmlRegisterUncreatableType<AbstractPlayerControl>("MediaPlayerControl", 1, 0, "AbstractPlayerControl", "");
    m_view->rootContext()->setContextProperty("core", this);
}

QStringList Core::playerControlsList() const
{
    return m_playerControls.keys();
}

AbstractPlayerControl *Core::playerControl(const QString &name)
{
    if (m_playerControls.contains(name))
        return m_playerControls[name];
    return 0;
}

AbstractPlayerControl *Core::playerControl()
{
    if (m_playerControls.contains("VLC"))
        return m_playerControls["VLC"];
    return 0;
}

void Core::cleanup()
{
    foreach(const QString &playerName, m_playerControls.keys()) {
        m_playerControls[playerName]->deInit();
    }
}

void Core::hideApplication()
{
#ifdef Q_OS_SYMBIAN
    RWindowGroup* wg = &CCoeEnv::Static()->RootWin();
    wg->SetOrdinalPosition(-1);
#endif
}

QString Core::humanizedTime(qulonglong time)
{
    int seconds = 0;
    int minutes = 0;
    int hours = 0;
    seconds = time%60;
    time /= 60;
    if (time) {
        minutes = time%60;
        hours = time/60;
    }
    QString result = QString("%1:%2:%3")
                     .arg(hours, 2, 10, QChar('0'))
                     .arg(minutes, 2, 10, QChar('0'))
                     .arg(seconds, 2, 10, QChar('0'));
    return result;

}

bool Core::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != m_view)
        return false;
    if (ev->type() == QEvent::WindowActivate)
        m_screenSaver.setScreenSaverInhibited(true);
    else if (ev->type() == QEvent::WindowDeactivate)
        m_screenSaver.setScreenSaverInhibited(false);
    return false;
}
