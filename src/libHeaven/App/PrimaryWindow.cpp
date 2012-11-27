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

#include <QDebug>
#include <QMenuBar>
#include <QApplication>
#include <QResizeEvent>

#include "libHeaven/Heaven.hpp"

#include "libHeaven/Views/Mode.h"
#include "libHeaven/Views/TopLevelWidget.h"

#include "libHeaven/App/Application.hpp"
#include "libHeaven/App/PrimaryWindow.hpp"
#include "libHeaven/App/PrimaryWindowPrivate.hpp"

#include "libHeaven/Actions/MenuBar.h"

#include "libHeaven/Widgets/ModeSwitchWidget.h"
#include "libHeaven/Widgets/FooterWidget.hpp"

#include "libHeaven/Style/Style.h"

namespace Heaven
{

    PrimaryWindowLayout::PrimaryWindowLayout( PrimaryWindowPrivate* owner )
        : QLayout( owner->mOwner )
        , mOwner( owner )
    {
        mTopLevel = new QWidgetItem( mOwner->mTopLevelWidget );
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

        shTop = mOwner->mTopLevelWidget->sizeHint();

        if( mOwner->mStatusBarWidget )
            shStatus = mOwner->mStatusBarWidget->sizeHint();

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
        mOwner = NULL;
        mMenuBar = NULL;
        mMenuBarWidget = NULL;
        mModeSwitchWidget = NULL;
        mTopLevelWidget = NULL;
        mStatusBarWidget = NULL;
    }

    void PrimaryWindowPrivate::setup()
    {
        mModeSwitchWidget = new ModeSwitchWidget();
        mTopLevelWidget = new TopLevelWidget( mOwner );

        QObject::connect( app(), SIGNAL(currentModeChanged(Heaven::Mode*)),
                          mModeSwitchWidget, SLOT(modeChanged(Heaven::Mode*)) );

        mLayout = new PrimaryWindowLayout( this );
        mOwner->setLayout( mLayout );

        QApplication::setStyle( new Style( QApplication::style() ) );
    }

    PrimaryWindow::PrimaryWindow()
        : QWidget()
        , d( new PrimaryWindowPrivate )
    {
        d->mOwner = this;
        d->setup();

        setProperty( "heavenStyle", true );
    }

    PrimaryWindow::~PrimaryWindow()
    {
        delete d;
    }

    MenuBar* PrimaryWindow::menuBar()
    {
        return d->mMenuBar;
    }

    void PrimaryWindow::setMenuBar( MenuBar* bar )
    {
        if( bar == d->mMenuBar )
        {
            return;
        }

        if( d->mMenuBarWidget )
        {
            Q_ASSERT( d->mModeSwitchWidget );
            d->mModeSwitchWidget->hide();
            d->mModeSwitchWidget->setParent( NULL );
            d->mMenuBarWidget->setCornerWidget( NULL );
            d->mMenuBarWidget->deleteLater();
            d->mMenuBarWidget = NULL;

            d->mLayout->setMenuBar( NULL );
        }

        d->mMenuBar = bar;
        if( d->mMenuBar )
        {
            Q_ASSERT( d->mModeSwitchWidget );
            d->mMenuBarWidget = d->mMenuBar->menuBarFor( this );
            d->mMenuBarWidget->setCornerWidget( d->mModeSwitchWidget );
            d->mModeSwitchWidget->show();
            d->mLayout->setMenuBar( d->mMenuBarWidget );
        }

        updateGeometry();
    }

    FooterWidget* PrimaryWindow::statusBar()
    {
        if( !d->mStatusBarWidget )
        {
            setStatusBar( new FooterWidget );
        }

        return d->mStatusBarWidget;
    }

    void PrimaryWindow::setStatusBar( FooterWidget* bar )
    {
        if( bar == d->mStatusBarWidget )
        {
            return;
        }

        if( d->mStatusBarWidget )
        {
            d->mStatusBarWidget->deleteLater();
            d->mStatusBarWidget = NULL;
            d->mLayout->setStatusBar( NULL );
        }

        if( bar )
        {
            d->mStatusBarWidget = bar;
            d->mStatusBarWidget->setParent( this );
            d->mStatusBarWidget->show();
            d->mLayout->setStatusBar( d->mStatusBarWidget );
        }

        updateGeometry();
    }

    TopLevelWidget* PrimaryWindow::topLevelContainer()
    {
        return d->mTopLevelWidget;
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

}
