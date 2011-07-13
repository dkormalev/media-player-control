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

#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QMetaType>
#include <QMap>
#include <QStringList>

class QDeclarativeView;
class AbstractPlayerControl;

class Core : public QObject
{
    Q_OBJECT
public:
    static Core *instance();
    void setDeclarativeView(QDeclarativeView *view);
    QStringList playerControlsList() const;
    AbstractPlayerControl *playerControl(const QString &name);
    Q_INVOKABLE AbstractPlayerControl *playerControl();
    Q_INVOKABLE QString humanizedTime(qulonglong time);

signals:

public slots:
    void cleanup();

private:
    explicit Core(QObject *parent = 0);
    Q_DISABLE_COPY(Core)
    static Core *m_instance;
    QDeclarativeView *m_view;
    QMap<QString, AbstractPlayerControl *> m_playerControls;
};

#endif // CORE_H
