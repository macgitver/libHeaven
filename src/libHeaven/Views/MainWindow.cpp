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

#include "Mode.h"
#include "MainWindow.h"
#include "MainWindowPrivate.h"

#include "libHeaven/Actions/MenuBar.h"

#include "libHeaven/Widgets/ModeSwitchWidget.h"
#include "libHeaven/Widgets/FooterWidget.hpp"
#include "libHeaven/Views/TopLevelWidget.h"

#include "libHeaven/Style/Style.h"

namespace Heaven
{

    MainWindowLayout::MainWindowLayout( MainWindowPrivate* owner )
        : QLayout( owner->mOwner )
        , mOwner( owner )
    {
        mTopLevel = new QWidgetItem( mOwner->mTopLevelWidget );
        mStatusBar = NULL;
    }

    void MainWindowLayout::addItem( QLayoutItem* item )
    {
        Q_ASSERT( false );
    }

    int MainWindowLayout::count() const
    {
        return mStatusBar ? 2 : 1;
    }

    QLayoutItem* MainWindowLayout::itemAt( int index ) const
    {
        if( index == 0 )
            return mTopLevel;

        if( index == 1 && mStatusBar )
            return mStatusBar;

        return NULL;
    }

    QLayoutItem* MainWindowLayout::takeAt( int index )
    {
        Q_ASSERT( false );
        return NULL;
    }

    QSize MainWindowLayout::sizeHint() const
    {
        QSize shTop, shStatus;

        shTop = mOwner->mTopLevelWidget->sizeHint();

        if( mOwner->mStatusBarWidget )
            shStatus = mOwner->mStatusBarWidget->sizeHint();

        int x = qMax( shTop.width(), shStatus.width() );
        int y = qMax( shTop.height(), shStatus.height() );

        return QSize( x, y );
    }

    QSize MainWindowLayout::maximumSize() const
    {
        return QLayout::maximumSize();
    }

    QSize MainWindowLayout::minimumSize() const
    {
        return QLayout::minimumSize();
    }

    void MainWindowLayout::setGeometry( const QRect& rect )
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

    void MainWindowLayout::setStatusBar( FooterWidget* bar )
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

    MainWindowPrivate::MainWindowPrivate()
    {
        mOwner = NULL;
        mCurrentMode = NULL;
        mMenuBar = NULL;
        mMenuBarWidget = NULL;
        mModeSwitchWidget = NULL;
        mTopLevelWidget = NULL;
        mStatusBarWidget = NULL;
    }

    void MainWindowPrivate::setup()
    {
        mModeSwitchWidget = new ModeSwitchWidget();
        mTopLevelWidget = new TopLevelWidget( mOwner );

        QObject::connect( mOwner, SIGNAL(currentModeChanged(Heaven::Mode*)),
                          mModeSwitchWidget, SLOT(modeChanged(Heaven::Mode*)) );

        mLayout = new MainWindowLayout( this );
        mOwner->setLayout( mLayout );

        QApplication::setStyle( new Style( QApplication::style() ) );
    }

    void MainWindowPrivate::switchToMode( Mode* mode )
    {
        if( mode == mCurrentMode )
        {
            return;
        }

        if( mCurrentMode )
        {
            mCurrentMode->deactivate();
            mCurrentMode = NULL;
        }

        if( mode )
        {
            mode->activate();
            mCurrentMode = mode;
        }

        emit mOwner->currentModeChanged( mode );
    }

    MainWindow::MainWindow()
        : QWidget()
        , d( new MainWindowPrivate )
    {
        d->mOwner = this;
        d->setup();

        setProperty( "heavenStyle", true );
    }

    MainWindow::~MainWindow()
    {
        delete d;
    }

    void MainWindow::setCurrentMode( Mode* mode )
    {
        d->switchToMode( mode );
    }

    Mode* MainWindow::findMode( const QString& name )
    {
        foreach( Mode* mode, d->mModes )
        {
            if( mode->name() == name )
            {
                return mode;
            }
        }

        return NULL;
    }

    void MainWindow::addMode( Mode* mode )
    {
        d->mModes.append( mode );
    }

    void MainWindow::removeMode( Mode* mode )
    {
        if( currentMode() == mode )
        {
            // FIXME: Find a suitable mode
            d->switchToMode( NULL );
        }

        int i = d->mModes.indexOf( mode );
        Q_ASSERT( i != -1 );
        d->mModes.remove( i );
        delete mode;
    }

    Mode* MainWindow::currentMode()
    {
        return d->mCurrentMode;
    }

    MenuBar* MainWindow::menuBar()
    {
        return d->mMenuBar;
    }

    void MainWindow::setMenuBar( MenuBar* bar )
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

    FooterWidget* MainWindow::statusBar()
    {
        if( !d->mStatusBarWidget )
        {
            setStatusBar( new FooterWidget );
        }

        return d->mStatusBarWidget;
    }

    void MainWindow::setStatusBar( FooterWidget* bar )
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

    TopLevelWidget* MainWindow::topLevelContainer()
    {
        return d->mTopLevelWidget;
    }

    bool MainWindow::event( QEvent* e )
    {
        switch( e->type() )
        {
        default:
            break;
        }

        return QWidget::event( e );
    }

}
