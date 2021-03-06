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


#include <QtGui/QApplication>
#include <QTextCodec>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include "core.h"

#if !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
# include <MDeclarativeCache>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(new QDeclarativeView());
#else
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());
#endif

    app->setOrganizationName("DenisKormalev");
    app->setApplicationName("MediaPlayerControl");
    app->setApplicationVersion("0.3.1");

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    Core::instance()->setDeclarativeView(view.data());

    view->installEventFilter(Core::instance());

    view->setSource(QUrl("qrc:/main.qml"));
    QObject::connect(view->engine(), SIGNAL(quit()), Core::instance(), SLOT(cleanup()));
    QObject::connect(view->engine(), SIGNAL(quit()), view.data(), SLOT(close()));
    view->showFullScreen();


    return app->exec();
}
