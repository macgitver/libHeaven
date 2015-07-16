/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QApplication>
#include <QVBoxLayout>

#include "libBlueSky/Dialog.hpp"
#include "libHeavenColors/ColorManager.hpp"
#include "libStairway/StairwayToHeavenTool.hpp"

#include "MainWindow.hpp"

MainWindow::MainWindow()
    : PrimaryWindow()
{
    Heaven::StairwayTool::StairwayView* v = new Heaven::StairwayTool::StairwayView;
    v->setRoot(this);
    v->show();
    v->setGeometry(0,0,800,800);
    setGeometry(850,0,700,800);

    setupActions(this);
    setMenuBar(mbMain);
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::settingsColors() {
    BlueSky::Dialog* d = new BlueSky::Dialog;
    d->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout* l = new QVBoxLayout;
    d->setLayout(l);
    l->addWidget(Heaven::ColorManager::self().createEditorWidget());

    d->exec();
}
