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

#include <QVBoxLayout>
#include <QTextBrowser>

#include "MainWindow.hpp"

#include "libHeaven/Views/ViewContainer.h"
#include "libHeaven/Views/GlobalView.hpp"

MainWindow::MainWindow()
{
    setupActions( this );
    setMenuBar( mbMain );
    statusBar();

    Heaven::ViewContainer* vc = rootContainer();

    Heaven::GlobalView* v1 = new Heaven::GlobalView( QLatin1String( "View1" ) );
    v1->setViewName( QLatin1String( "View1" ) );
    QTextBrowser* t1 = new QTextBrowser;
    QVBoxLayout* l1 = new QVBoxLayout;
    l1->addWidget( t1 );
    v1->setLayout( l1 );
    t1->setText( QLatin1String( "<b>View 1 -> TextBrowser 1</b>" ) );
    vc->add( v1 );

    Heaven::GlobalView* v2 = new Heaven::GlobalView( QLatin1String( "View2" ) );
    v2->setViewName( QLatin1String( "View2" ) );
    QTextBrowser* t2 = new QTextBrowser;
    QVBoxLayout* l2 = new QVBoxLayout;
    l2->addWidget( t2 );
    v2->setLayout( l2 );
    t2->setText( QLatin1String( "<b>View 2 -> TextBrowser 2</b>" ) );
    vc->add( v2 );
}

