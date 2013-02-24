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

#include <QPainter>
#include <QPalette>
#include <QVBoxLayout>
#include <QQueue>

#include <QUuid>

#include "libHeaven/HeavenPrivate.hpp"

#include "libHeaven/App/HeavenWindow.hpp"
#include "libHeaven/Views/TopLevelWidget.h"

namespace Heaven
{

    TopLevelWidget::TopLevelWidget( HeavenWindow* parent )
        : QWidget( parent )
    {
        QString handle = QLatin1String( UUIDSTR_PRIMARY_SPLITTER );

        if( !parent->isPrimary() )
        {
            handle = QUuid::createUuid().toString();
        }

        mRoot = new ViewContainer( handle,
                                   ViewContainer::Splitter,
                                   ViewContainer::SubSplitHorz );

        QVBoxLayout* l = new QVBoxLayout;
        l->setMargin( 0 );
        l->setSpacing( 0 );
        l->addWidget( mRoot->containerWidget() );
        setLayout( l );

        setAutoFillBackground( false );
    }

    TopLevelWidget::~TopLevelWidget()
    {
        // TODO: Remove this; it should never do anything (By the time the TLW gets destroied, there
        // ought to be no views in it.
        mRoot->clear();
    }

    ViewContainer* TopLevelWidget::rootContainer()
    {
        return mRoot;
    }

}
