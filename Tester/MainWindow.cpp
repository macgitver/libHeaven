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
#include <QDialogButtonBox>
#include <QDebug>
#include <QLabel>
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
    v->setGeometry(0,20,500,700);
    setGeometry(520,20,500,700);

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

    // a heavenly size grip for testing
    d->setResizerEnabled(true);

    d->exec();
}

void MainWindow::showPinnedDialog()
{
    BlueSky::Dialog *dlg = new BlueSky::Dialog;
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setResizerEnabled(true);

    QVBoxLayout* l = new QVBoxLayout;
    dlg->setLayout(l);

    QLabel* sampleText =
            new QLabel(
                QStringLiteral("<html><head><style>"
                               "h1 {color: #ea5}"
                               "pre {margin: 10px; background: #333; color: #fed; font: monospace 13pt;}"
                               "</style></head><body>"
                               "<h1>Pinned Dialog Example</h1>"
                               "<p>"
                               "This is a sample for a libHeaven pinned Dialog."
                               "<br/><br/>A pinned dalog is created like this:"
                               "<pre lang='c++'`>"
                               "BlueSky::Dialog *dlg = new BlueSky::Dialog;"
                               "\ndlg->setAttribute(Qt::WA_DeleteOnClose);"
                               "\n// setup the dialog contents here"
                               "\ndlg->open();\n_"
                               "</pre></p></body></html>"
                               ));
    l->addWidget(sampleText);
    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttons, &QDialogButtonBox::rejected, dlg, &BlueSky::Dialog::reject);
    l->addWidget(buttons);

    dlg->setStyleSheet(QLatin1Literal("QLabel{ border: 1px dotted gray; background: #eef; }"));

    dlg->open();

    // some output for testing
    const QPoint absPos = dlg->frameGeometry().topLeft();
    qDebug() << "Pinned Dialog:"
             << "\n  - parentWidget == this:\t" << (dlg->parentWidget() == this)
             << "\n  - modality:\t" << dlg->windowModality()
             << "\n  - isModal:\t" << dlg->isModal()
             << "\n  - abs. positiion:\t"   << absPos
             << "\n  - rel. position:\t"    << mapFromParent(absPos)
             << "\n  - size:\t"             << dlg->size()
                ;
}
