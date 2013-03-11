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
#include <QMenuBar>
#include <QApplication>
#include <QResizeEvent>

#include "libHeaven/Heaven.hpp"
#include "libHeaven/HeavenPrivate.hpp"

#include "libHeaven/App/Mode.hpp"
#include "libHeaven/App/Application.hpp"
#include "libHeaven/App/ApplicationPrivate.hpp"
#include "libHeaven/App/PrimaryWindow.hpp"
#include "libHeaven/App/PrimaryWindowPrivate.hpp"

#include "libHeaven/Actions/MenuBar.h"

#include "libHeaven/Widgets/ModeSwitchWidget.hpp"
#include "libHeaven/Widgets/FooterWidget.hpp"

#include "libHeaven/CentralUI/ContainerWidgets/SplitterContainerWidget.hpp"

#include "libHeaven/Style/Style.hpp"

namespace Heaven
{

    PrimaryWindowLayout::PrimaryWindowLayout( PrimaryWindowPrivate* owner )
        : QLayout( owner->owner )
        , mOwner( owner )
    {
        mTopLevel = new QWidgetItem( mOwner->root );
        mStatusBar = NULL;
    }

    void PrimaryWindowLayout::addItem( QLayoutItem* item )
    {
        Q_ASSERT( false );
    }

    int PrimaryWindowLayout::count() const
    {
        return mStatusBar ? 2 : 1;
    }

    QLayoutItem* PrimaryWindowLayout::itemAt( int index ) const
    {
        if( index == 0 )
            return mTopLevel;

        if( index == 1 && mStatusBar )
            return mStatusBar;

        return NULL;
    }

    QLayoutItem* PrimaryWindowLayout::takeAt( int index )
    {
        Q_ASSERT( false );
        return NULL;
    }

    QSize PrimaryWindowLayout::sizeHint() const
    {
        QSize shTop, shStatus;

        shTop = mOwner->root->sizeHint();

        if( mOwner->statusBarWidget )
        {
            shStatus = mOwner->statusBarWidget->sizeHint();
        }

        int x = qMax( shTop.width(), shStatus.width() );
        int y = qMax( shTop.height(), shStatus.height() );

        return QSize( x, y );
    }

    QSize PrimaryWindowLayout::maximumSize() const
    {
        return QLayout::maximumSize();
    }

    QSize PrimaryWindowLayout::minimumSize() const
    {
        return QLayout::minimumSize();
    }

    void PrimaryWindowLayout::setGeometry( const QRect& rect )
    {
        QRect tl( rect );

        if( mStatusBar )
        {
            QSize sh = mStatusBar->sizeHint();
            mStatusBar->setGeometry( QRect( rect.left(), rect.bottom() - sh.height(),
                                     rect.width(), sh.height() ) );
            tl.setHeight( tl.height() - sh.height() );
        }

        mTopLevel->setGeometry( tl );
    }

    void PrimaryWindowLayout::setStatusBar( FooterWidget* bar )
    {
        if( mStatusBar )
        {
            delete mStatusBar;
            mStatusBar = NULL;
        }

        if( bar )
        {
            mStatusBar = new QWidgetItem( bar );
        }

        invalidate();
    }

    PrimaryWindowPrivate::PrimaryWindowPrivate()
    {
        owner = NULL;
        menuBar = NULL;
        menuBarWidget = NULL;
        modeSwitchWidget = NULL;
        statusBarWidget = NULL;

        root = NULL;
        handle = QLatin1String( "#" );
    }

    void PrimaryWindowPrivate::setup()
    {
        root = new SplitterContainerWidget( ViewIdentifier( UUIDSTR_PRIMARY_SPLITTER ), false );
        root->setParent( owner );

        modeSwitchWidget = new ModeSwitchWidget();

        QObject::connect( app(), SIGNAL(currentModeChanged(Heaven::Mode*)),
                          modeSwitchWidget, SLOT(modeChanged(Heaven::Mode*)) );

        layout = new PrimaryWindowLayout( this );
        owner->setLayout( layout );

        QApplication::setStyle( new Style( QApplication::style() ) );
    }

    PrimaryWindow::PrimaryWindow()
        : HeavenWindow( new PrimaryWindowPrivate )
    {
        HWPD(PrimaryWindow);

        if( Application::self()->primaryWindow() )
        {
            qFatal( "Only one PrimaryWindow is allowed at a time" );
        }

        d->owner = this;
        d->setup();

        setProperty( "heavenStyle", true );

        setHandle( ViewIdentifier( UUIDSTR_PRIMARY_WINDOW ) );

        ApplicationPrivate::setPrimaryWindow( this );
    }

    PrimaryWindow::~PrimaryWindow()
    {
        ApplicationPrivate::setPrimaryWindow( NULL );
    }

    MenuBar* PrimaryWindow::menuBar() const
    {
        HWPD(const PrimaryWindow);
        return d->menuBar;
    }

    void PrimaryWindow::setMenuBar( MenuBar* bar )
    {
        HWPD(PrimaryWindow);

        if( bar == d->menuBar )
        {
            return;
        }

        if( d->menuBarWidget )
        {
            Q_ASSERT( d->modeSwitchWidget );
            d->modeSwitchWidget->hide();
            d->modeSwitchWidget->setParent( NULL );
            d->menuBarWidget->setCornerWidget( NULL );
            d->menuBarWidget->deleteLater();
            d->menuBarWidget = NULL;

            d->layout->setMenuBar( NULL );
        }

        d->menuBar = bar;
        if( d->menuBar )
        {
            Q_ASSERT( d->modeSwitchWidget );
            d->menuBarWidget = d->menuBar->menuBarFor( this );
            d->menuBarWidget->setCornerWidget( d->modeSwitchWidget );
            d->modeSwitchWidget->show();
            d->layout->setMenuBar( d->menuBarWidget );
        }

        updateGeometry();
    }

    FooterWidget* PrimaryWindow::statusBar() const
    {
        HWPD( const PrimaryWindow );

        if( !d->statusBarWidget )
        {
            PrimaryWindow* t = const_cast< PrimaryWindow* >( this );
            t->setStatusBar( new FooterWidget );
            return d->statusBarWidget;
        }

        return d->statusBarWidget;
    }

    void PrimaryWindow::setStatusBar( FooterWidget* bar )
    {
        HWPD( PrimaryWindow );

        if( bar == d->statusBarWidget )
        {
            return;
        }

        if( d->statusBarWidget )
        {
            d->statusBarWidget->deleteLater();
            d->statusBarWidget = NULL;
            d->layout->setStatusBar( NULL );
        }

        if( bar )
        {
            d->statusBarWidget = bar;
            d->statusBarWidget->setParent( this );
            d->statusBarWidget->show();
            d->layout->setStatusBar( d->statusBarWidget );
        }

        updateGeometry();
    }

    bool PrimaryWindow::event( QEvent* e )
    {
        switch( e->type() )
        {
        default:
            break;
        }

        return QWidget::event( e );
    }

    bool PrimaryWindow::isPrimary() const
    {
        return true;
    }

}
