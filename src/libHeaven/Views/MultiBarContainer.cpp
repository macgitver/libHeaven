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

#include <QBoxLayout>
#include <QStackedWidget>

#include "libHeaven/Views/ViewContainerContent.h"
#include "libHeaven/Views/MultiBarContainer.hpp"

#include "libHeaven/Widgets/MultiBar.hpp"

namespace Heaven
{

    static inline bool isVertical( MultiBarContainer::BarPos pos )
    {
        return ( pos == MultiBarContainer::West ) || ( pos == MultiBarContainer::East );
    }

    class MultiBarContainerPrivate
    {
    public:
        MultiBarContainerPrivate();

    public:
        void relayout();
        void updateViewsSection();

    public:
        MultiBarContainer*              owner;
        MultiBarContainer::BarPos       barPos;
        QList< ViewContainerContent* >  views;
        ViewContainerContent*           active;
        QStackedWidget*                 stack;
        MultiBar*                       toolingBar;
        MultiBar*                       viewsBar;
        QVBoxLayout*                    layout;
    };

    MultiBarContainerPrivate::MultiBarContainerPrivate()
    {
        barPos = MultiBarContainer::North;
        owner = NULL;
        layout = NULL;
        stack = NULL;
        active = NULL;
        toolingBar = viewsBar = NULL;
    }

    void MultiBarContainerPrivate::relayout()
    {
        delete layout;

        layout = new QVBoxLayout( owner );
        layout->setMargin( 0 );
        layout->setSpacing( 0 );

        layout->addWidget( toolingBar );

        if( isVertical( barPos ) )
        {
            QHBoxLayout* l = new QHBoxLayout;
            l->setMargin( 0 );
            l->setSpacing( 0 );

            if( barPos == MultiBarContainer::West )
            {
                l->addWidget( viewsBar );
            }

            l->addWidget( stack );

            if( barPos == MultiBarContainer::East )
            {
                l->addWidget( viewsBar );
            }

            layout->addLayout( l );
        }
        else
        {
            layout->addWidget( stack );
        }

        if( barPos == MultiBarContainer::South )
        {
            layout->addWidget( viewsBar );
        }

        layout->activate();
        owner->update();
    }

    void MultiBarContainerPrivate::updateViewsSection()
    {
    }

    MultiBarContainer::MultiBarContainer()
    {
        d = new MultiBarContainerPrivate;

        d->owner = this;
        d->toolingBar = d->viewsBar = new MultiBar;

        d->relayout();
    }

    MultiBarContainer::~MultiBarContainer()
    {
        delete d;
    }

    MultiBarContainer::BarPos MultiBarContainer::barPos() const
    {
        return d->barPos;
    }

    void MultiBarContainer::setBarPos( MultiBarContainer::BarPos position )
    {
        if( position != d->barPos )
        {
            if( d->barPos != North )
            {
                // takeViewsSection()
                delete d->viewsBar;
                d->viewsBar = NULL;
            }

            d->barPos = position;

            if( d->barPos != North )
            {
                d->viewsBar = new MultiBar;
                // insertViewsSection()
            }

            d->relayout();
        }
    }

    int MultiBarContainer::addView( ViewContainerContent* view )
    {
        return insertView( d->views.count(), view );
    }

    int MultiBarContainer::insertView( int index, ViewContainerContent* view )
    {
        return -1;
    }

    ViewContainerContent* MultiBarContainer::takeView( int index )
    {
        if( index < 0 || index >= d->views.count() )
        {
            return NULL;
        }

        QWidget* w = d->stack->widget( index );
        if( !w )
        {
            return NULL;
        }

        ViewContainerContent* vcc = d->views.at( index );
        if( !vcc )
        {
            return NULL;
        }

        Q_ASSERT( w == vcc->widget() );

        w->hide();
        w->setParent( NULL );   // This will remove it from mStack!

        d->views.removeAt( index );

        d->updateViewsSection();

        return vcc;
    }

}
