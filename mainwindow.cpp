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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core.h"

#include <QtCore/QCoreApplication>
#include <QDeclarativeView>

#ifdef Q_OS_SYMBIAN
# include <w32std.h>
# include <mw/coemain.h>
#endif


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifdef Q_OS_SYMBIAN
    menuBar()->addAction("Settings", this, SLOT(showSettings()));
    menuBar()->addAction("Quit", this, SLOT(quitApplication()));
    centralWidget()->setContextMenuPolicy(Qt::NoContextMenu);
    QAction *hideAction = new QAction(tr("Hide"), this);
    connect(hideAction, SIGNAL(triggered()), this, SLOT(hideApplication()));
    hideAction->setSoftKeyRole(QAction::NegativeSoftKey);
    addAction(hideAction);
#else
    QMenu *fileMenu = menuBar()->addMenu("File");
    //fileMenu->addAction("Settings", this, SLOT(showSettings()));
    fileMenu->addAction("Quit", this, SLOT(quitApplication()));
#endif


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::setSource(const QUrl &uri)
{
    ui->centralWidget->setSource(uri);
}

void MainWindow::setupCore()
{
    Core::instance()->setDeclarativeView(ui->centralWidget);
    connect(qApp, SIGNAL(destroyed()), Core::instance(), SLOT(cleanup()));
}

void MainWindow::quitApplication()
{
    qApp->quit();
}

void MainWindow::hideApplication()
{
#ifdef Q_OS_SYMBIAN
    RWindowGroup* wg = &CCoeEnv::Static()->RootWin();
    wg->SetOrdinalPosition(-1);
#endif
}
