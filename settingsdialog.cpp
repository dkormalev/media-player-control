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

#include "settingsdialog.h"
#include "core.h"
#include "abstractplayercontrol.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QAction>


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    setLayout(new QVBoxLayout());
    layout()->setSpacing(10);
    setStyleSheet("QLineEdit {height: 65; font-size: 22px;} QLabel {font-size: 26px;}");

#ifdef Q_OS_SYMBIAN
    QAction *okAction = new QAction(tr("Ok"), this);
    connect(okAction, SIGNAL(triggered()), this, SLOT(okClicked()));
    okAction->setSoftKeyRole(QAction::PositiveSoftKey);
    addAction(okAction);
    QAction *cancelAction = new QAction(tr("Cancel"), this);
    connect(cancelAction, SIGNAL(triggered()), this, SLOT(close()));
    cancelAction->setSoftKeyRole(QAction::NegativeSoftKey);
    addAction(cancelAction);
#endif

}

void SettingsDialog::initDialog()
{
    QLayoutItem *child;
    while ((child = layout()->takeAt(0)) != 0)
    {
        QWidget *childWidget = child->widget();
        delete child;
        if (childWidget)
            childWidget->deleteLater();
    }

    AbstractPlayerControl *currentPlayerControl = Core::instance()->playerControl();
    QBoxLayout *layoutItem = qobject_cast<QBoxLayout *>(layout());

    foreach(const QString &paramName, currentPlayerControl->params()) {
        layout()->addWidget(new QLabel(paramName));
        QLineEdit *edit = new QLineEdit(currentPlayerControl->param(paramName));
        m_controls[paramName] = edit;
        layout()->addWidget(edit);
        if (layoutItem) {
            layoutItem->addSpacing(20);
        }
    }

#ifndef Q_OS_SYMBIAN
    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Cancel");

    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    if (layoutItem) {
        layoutItem->addSpacing(20);
    }
    layout()->addWidget(okButton);
    layout()->addWidget(cancelButton);
#endif
    if (layoutItem) {
        QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        layoutItem->addSpacerItem(spacer);
    }
}

void SettingsDialog::okClicked()
{
    AbstractPlayerControl *currentPlayerControl = Core::instance()->playerControl();
    foreach(const QString &paramName, m_controls.keys()) {
        currentPlayerControl->setParam(paramName, m_controls[paramName]->text());
    }
    close();
}
