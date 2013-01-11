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

#include <QDebug>
#include <QBoxLayout>
#include <QStackedWidget>

#include "libHeaven/Views/ViewContainerContent.h"
#include "libHeaven/Views/View.h"

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

        void clearToolBar();
        void setupToolBar();

        enum ContainerAction
        {
            None                = 0,
            CloseView           = 1 << 0,
            MaximizeView        = 1 << 1,
            MinimizeView        = 1 << 2
        };
        typedef QFlags< ContainerAction > ContainerActions;

        void updateActions();

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
        ContainerActions                possibileActions;
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
        if( active )
        {
            possibileActions |= CloseView;
        }
        else
        {
            possibileActions &= ~CloseView;
        }
    }

    void MultiBarContainerPrivate::updateActions()
    {
        #define UPDATE_ACTION(Action,Flag,Extra) \
            do { \
                (Action)->setVisible( possibileActions.testFlag( Flag ) && (Extra) ); \
            } while(0)

        UPDATE_ACTION( actClose,        CloseView,      true );
        UPDATE_ACTION( actMaximizeV,    MaximizeView,   true );
        UPDATE_ACTION( actMinimizeV,    MinimizeView,   true );

        #undef UPDATE_ACTION
    }

    void MultiBarContainerPrivate::clearToolBar()
    {
        userToolBar->setToolBar( NULL );
    }

    void MultiBarContainerPrivate::setupToolBar()
    {
        if( !active )
        {
            clearToolBar();
            return;
        }

        Q_ASSERT( !active->isContainer() );
        View* v = active->asView();
        userToolBar->setToolBar( v->toolBar() );
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

            if( isVertical( d->barPos ) )
            {
                d->viewsBar->setOrientation( Qt::Vertical );
                d->viewsSection->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
            }
            else
            {
                d->viewsBar->setOrientation( Qt::Horizontal );
                d->viewsSection->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
            }

            d->relayout();
            d->updateActions();
        }
    }

    int MultiBarContainer::addView( ViewContainerContent* view )
    {
        return insertView( d->views.count(), view );
    }

    int MultiBarContainer::insertView( int index, ViewContainerContent* view )
    {
        Q_ASSERT( !view->isContainer() );

        d->views.insert( index, view );
        d->stack->insertWidget( index, view->widget() );
        d->viewsSection->insertView( index, view->asView() );

        connect( view->asView(), SIGNAL(toolBarChanged(Heaven::ToolBar*)),
                 this, SLOT(viewToolBarChanged(Heaven::ToolBar*)) );

        if( d->views.count() == 1 )
        {
            d->active = view;
            d->setupToolBar();
        }

        d->updateViewsSection();
        d->updateActions();

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
        d->updateActions();

        return vcc;
    }

    void MultiBarContainer::viewChanged( int index )
    {
        d->active = ( index == -1 ) ? NULL : d->views.at( index );
        d->stack->setCurrentIndex( index );
        d->setupToolBar();
        d->updateViewsSection();
        d->updateActions();
    }

    void MultiBarContainer::viewToolBarChanged( Heaven::ToolBar* toolBar )
    {
        Q_UNUSED( toolBar );

        View* view = qobject_cast< View* >( sender() );
        if( !view )
        {
            return;
        }

        int viewIndex = d->views.indexOf( view );
        if( viewIndex == -1 || view != d->active )
        {
            return;
        }

        d->setupToolBar();
    }

    void MultiBarContainer::onCloseActiveView()
    {
        if( !d->active )
        {
            return;
        }

        d->active->asView()->closeView();
    }

}
