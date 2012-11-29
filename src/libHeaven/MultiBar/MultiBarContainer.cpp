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

#include "libHeaven/MultiBar/MultiBarContainer.hpp"
#include "libHeaven/MultiBar/MultiBar.hpp"
#include "libHeaven/MultiBar/MultiBarViewSection.hpp"
#include "libHeaven/MultiBar/MultiBarToolSection.hpp"

#include "hic_MultiBarContainerActions.h"

namespace Heaven
{

    static inline bool isVertical( MultiBarContainer::BarPos pos )
    {
        return ( pos == MultiBarContainer::West ) || ( pos == MultiBarContainer::East );
    }

    class MultiBarContainerPrivate : public MultiBarContainerActions
    {
    public:
        MultiBarContainerPrivate( MultiBarContainer* aOwner );

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
        MultiBarViewSection*            viewsSection;
        MultiBarToolSection*            userToolBar;
        MultiBarToolSection*            adminToolBar;
        QVBoxLayout*                    layout;
    };

    MultiBarContainerPrivate::MultiBarContainerPrivate( MultiBarContainer* aOwner )
    {
        barPos = MultiBarContainer::North;
        owner = aOwner;
        layout = NULL;
        active = NULL;
        stack = new QStackedWidget;
        toolingBar = viewsBar = new MultiBar;
        viewsSection = new MultiBarViewSection;

        viewsBar->addSection( viewsSection );

        QObject::connect( viewsSection, SIGNAL(currentChanged(int)),
                          owner, SLOT(viewChanged(int)) );

        setupActions( owner );

        adminToolBar = new MultiBarToolSection;
        adminToolBar->setToolBar( tbDefaultV );

        userToolBar = new MultiBarToolSection;
        userToolBar->setStretch( 1 );

        toolingBar->addSection( userToolBar );
        toolingBar->addSection( adminToolBar );

        relayout();
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
        d = new MultiBarContainerPrivate( this );
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
            if( d->barPos != North && position == North )
            {
                // We're moving from somewhere TO North
                // => We don't need the viewsBar any longer

                d->viewsBar->takeSection( 0 );

                delete d->viewsBar;
                d->viewsBar = d->toolingBar;

                d->toolingBar->insertSection( 0, d->viewsSection );
            }
            else if( d->barPos == North )
            {
                // We're moving from North TO somewhere
                // => We now need a viewsBar at that side

                d->toolingBar->takeSection( 0 );

                d->viewsBar = new MultiBar;

                d->viewsBar->insertSection( 0, d->viewsSection );
            }

            d->barPos = position;

            d->viewsBar->setOrientation( isVertical( d->barPos ) ? Qt::Vertical : Qt::Horizontal );
            d->relayout();
        }
    }

    int MultiBarContainer::addView( ViewContainerContent* view )
    {
        return insertView( d->views.count(), view );
    }

    int MultiBarContainer::insertView( int index, ViewContainerContent* view )
    {
        d->views.insert( index, view );
        d->stack->insertWidget( index, view->widget() );
        d->viewsSection->insertView( index, view->asView() );

        return index;
    }

    ViewContainerContent* MultiBarContainer::takeView( int index )
    {
        if( index < 0 || index >= d->views.count() )
        {
            return NULL;
        }

        ViewContainerContent* vcc = d->views.at( index );
        if( !vcc )
        {
            return NULL;
        }

        QWidget* w = d->stack->widget( index );
        if( !w )
        {
            return NULL;
        }

        Q_ASSERT( w == vcc->widget() );

        w->hide();
        w->setParent( NULL );

        d->views.removeAt( index );
        d->viewsSection->removeView( vcc->asView() );

        d->updateViewsSection();

        return vcc;
    }

    void MultiBarContainer::viewChanged( int index )
    {
        // todo: do the toolbar foo here
        d->stack->setCurrentIndex( index );
    }

}
