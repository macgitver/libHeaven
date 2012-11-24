/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QStyleOption>
#include <QPainter>
#include <QBoxLayout>

#include "MultiBar/MultiBar.hpp"
#include "MultiBar/MultiBarSection.hpp"
#include "MultiBar/MultiBarPrivate.hpp"

namespace Heaven
{

    MultiBarPrivate::MultiBarPrivate()
        : owner( NULL )
        , layout( NULL )
        , orientation( Qt::Horizontal )
    {
    }

    void MultiBarPrivate::relayout()
    {
        QBoxLayout* inner = NULL;

        delete layout;

        if( orientation == Qt::Horizontal )
        {
            layout = new QVBoxLayout( owner );
            inner = new QHBoxLayout();
        }
        else
        {
            layout = new QHBoxLayout( owner );
            inner = new QVBoxLayout();
        }

        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        inner->setMargin( 0 );
        inner->setSpacing( 0 );
        inner->addSpacing( 2 );

        for( int i = 0; i < sections.count(); i++ )
        {
            MultiBarSection* section = sections.at( i );
            inner->addWidget( section );
        }

        inner->addSpacing( 2 );

        layout->addSpacing( 2 );
        layout->addLayout( inner );
        layout->addSpacing( 2 );

        layout->activate();
        owner->update();
    }

    MultiBar::MultiBar( QWidget* parent )
        : QWidget( parent )
    {
        d = new MultiBarPrivate;
        d->owner = this;
        d->relayout();
    }

    MultiBar::~MultiBar()
    {
        removeAllSections();
        delete d;
    }

    void MultiBar::setOrientation( Qt::Orientation orientation )
    {
        if( orientation != d->orientation )
        {
            d->orientation = orientation;

            foreach( MultiBarSection* sect, d->sections )
            {
                sect->setOrientation( orientation );
            }

            d->relayout();
        }
    }

    Qt::Orientation MultiBar::orientation() const
    {
        return d->orientation;
    }

    int MultiBar::sectionCount() const
    {
        return d->sections.count();
    }

    int MultiBar::addSection( MultiBarSection* section )
    {
        d->sections.append( section );
        section->setParent( this );
        section->setOrientation( d->orientation );
        section->show();
        d->relayout();
        return d->sections.count() - 1;
    }

    int MultiBar::addSection()
    {
        return addSection( new MultiBarSection( this ) );
    }

    MultiBarSection* MultiBar::takeSection( int index )
    {
        MultiBarSection* sect = d->sections.at( index );
        d->sections.removeAt( index );

        if( sect )
        {
            sect->hide();
            sect->setParent( NULL );
        }

        d->relayout();

        return sect;
    }

    void MultiBar::insertSection( int index, MultiBarSection* section )
    {
        Q_ASSERT( section );

        d->sections.insert( index, section );
        section->setParent( this );
        section->setOrientation( d->orientation );
        section->show();
        d->relayout();
    }

    void MultiBar::removeSection( int index )
    {
        d->sections.removeAt( index );
        d->relayout();
    }

    void MultiBar::removeAllSections()
    {
        while( d->sections.count() )
        {
            removeSection( 0 );
        }
    }

    void MultiBar::paintEvent( QPaintEvent* ev )
    {
        QPainter p( this );
        QStyleOption opt;
        opt.initFrom( this );

        if( d->orientation == Qt::Horizontal )
            opt.state |= QStyle::State_Horizontal;

        style()->drawPrimitive( QStyle::PE_PanelToolBar, &opt, &p, this );
    }

    QSize MultiBar::minimumSizeHint() const
    {
        return d->orientation == Qt::Horizontal
                ? QSize( 0, 22 )
                : QSize( 22, 0 );
    }

}
