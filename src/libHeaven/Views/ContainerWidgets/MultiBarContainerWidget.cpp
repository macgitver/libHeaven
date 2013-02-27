/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#include "libHeaven/MultiBar/MultiBar.hpp"
#include "libHeaven/MultiBar/MultiBarViewSection.hpp"
#include "libHeaven/MultiBar/MultiBarToolSection.hpp"

#include "libHeaven/Views/View.h"
#include "libHeaven/Views/ContainerWidgets/MultiBarContainerWidget.hpp"

#include "hic_MultiBarContainerWidgetActions.h"

namespace Heaven
{

    static inline bool isVertical( Positions pos )
    {
        return ( pos == Left ) ||
               ( pos == Right );
    }

    class MultiBarContainerWidgetPrivate : public MultiBarContainerWidgetActions
    {
    public:
        MultiBarContainerWidgetPrivate( MultiBarContainerWidget* aOwner );

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
        MultiBarContainerWidget*    owner;
        Positions                   barPos;
        AbstractViewWidget*         active;
        QStackedWidget*             stack;
        MultiBar*                   toolingBar;
        MultiBar*                   viewsBar;
        MultiBarViewSection*        viewsSection;
        MultiBarToolSection*        userToolBar;
        MultiBarToolSection*        adminToolBar;
        QVBoxLayout*                layout;
        ContainerActions            possibileActions;
    };

    MultiBarContainerWidgetPrivate::MultiBarContainerWidgetPrivate( MultiBarContainerWidget* aOwner )
    {
        barPos = Top;
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

    void MultiBarContainerWidgetPrivate::relayout()
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

            if( barPos == Left )
            {
                l->addWidget( viewsBar );
            }

            l->addWidget( stack );

            if( barPos == Right )
            {
                l->addWidget( viewsBar );
            }

            layout->addLayout( l );
        }
        else
        {
            layout->addWidget( stack );
        }

        if( barPos == Bottom )
        {
            layout->addWidget( viewsBar );
        }

        layout->activate();
        owner->update();
    }

    void MultiBarContainerWidgetPrivate::updateViewsSection()
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

    void MultiBarContainerWidgetPrivate::updateActions()
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

    void MultiBarContainerWidgetPrivate::clearToolBar()
    {
        userToolBar->setToolBar( NULL );
    }

    void MultiBarContainerWidgetPrivate::setupToolBar()
    {
        if( !active )
        {
            clearToolBar();
            return;
        }

        Q_ASSERT( !active->isContainerWidget() );
        View* v = qobject_cast< View* >( active );
        if( v )
        {
            userToolBar->setToolBar( v->toolBar() );
        }
    }

    MultiBarContainerWidget::MultiBarContainerWidget( const QString& identifier )
        : ContainerWidget( identifier )
    {
        d = new MultiBarContainerWidgetPrivate( this );
    }

    MultiBarContainerWidget::~MultiBarContainerWidget()
    {
        delete d;
    }

    Positions MultiBarContainerWidget::barPosition() const
    {
        return d->barPos;
    }

    void MultiBarContainerWidget::setBarPosition( Positions position )
    {
        if( position != d->barPos )
        {
            if( d->barPos != Top && position == Top )
            {
                // We're moving from somewhere TO North
                // => We don't need the viewsBar any longer

                d->viewsBar->takeSection( 0 );

                delete d->viewsBar;
                d->viewsBar = d->toolingBar;

                d->toolingBar->insertSection( 0, d->viewsSection );
            }
            else if( d->barPos == Top )
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

    int MultiBarContainerWidget::insert( int index, AbstractViewWidget* viewWidget )
    {
        View* view = qobject_cast< View* >( viewWidget );
        Q_ASSERT( view );

        d->stack->insertWidget( index, viewWidget );
        d->viewsSection->insertView( index, view );

        connect( view, SIGNAL(toolBarChanged(Heaven::ToolBar*)),
                 this, SLOT(viewToolBarChanged(Heaven::ToolBar*)) );

        if( d->stack->count() == 1 )
        {
            d->active = viewWidget;
            d->setupToolBar();
        }

        d->updateViewsSection();
        d->updateActions();

        view->queueRelayouting();

        return index;
    }

    AbstractViewWidget* MultiBarContainerWidget::takeAt( int index )
    {
        if( index < 0 || index >= d->stack->count() )
        {
            return NULL;
        }

        View* view = qobject_cast< View* >( d->stack->widget( index ) );
        if( !view )
        {
            return NULL;
        }

        view->hide();
        view->setParent( NULL );  // removes it from d->stack

        d->viewsSection->removeView( qobject_cast< View* >( view ) );

        d->updateViewsSection();
        d->updateActions();

        view->queueRelayouting();

        return view;
    }

    int MultiBarContainerWidget::indexOf( AbstractViewWidget* widget ) const
    {
        return d->stack->indexOf( widget );
    }

    AbstractViewWidget* MultiBarContainerWidget::widget( int index )
    {
        return static_cast< AbstractViewWidget* >( d->stack->widget( index ) );
    }

    int MultiBarContainerWidget::count() const
    {
        return d->stack->count();
    }

    void MultiBarContainerWidget::viewChanged( int index )
    {
        d->active = ( index == -1 ) ? NULL
                                    : qobject_cast< AbstractViewWidget* >( d->stack->widget( index ) );
        d->stack->setCurrentIndex( index );

        d->setupToolBar();
        d->updateViewsSection();
        d->updateActions();
    }

    void MultiBarContainerWidget::viewToolBarChanged( Heaven::ToolBar* toolBar )
    {
        Q_UNUSED( toolBar );

        View* view = qobject_cast< View* >( sender() );
        if( !view )
        {
            return;
        }

        int viewIndex = d->stack->indexOf( view );
        if( viewIndex == -1 || view != d->active )
        {
            return;
        }

        d->setupToolBar();
    }

    void MultiBarContainerWidget::onCloseActiveView()
    {
        View* view = qobject_cast< View* >( d->active );
        if( view )
        {
            view->closeView();
        }
    }

    ContainerTypes MultiBarContainerWidget::containerType() const
    {
        return MultiBarContainerType;
    }

}
