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

#include "libHeavenActions/MenuBar.hpp"

#include "libHeaven/Widgets/ModeSwitchWidget.hpp"
#include "libHeaven/Widgets/FooterWidget.hpp"

#include "libHeaven/CentralUI/ContainerWidgets/SplitterContainerWidget.hpp"

#include "libHeaven/Style/Style.hpp"

namespace Heaven
{

    /**
     * @class       PrimaryWindow
     * @brief       A Heaven-Application's primary window
     *
     * A primary (main) window is the window, which provides a menu bar and footer widget.
     *
     * On Mac OS the menu bar integrates natively; on all other platforms, the menu bar is shown
     * at the top of the window.
     *
     */

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

    /**
     * @brief       Constructor
     *
     * Creates the primary window.
     *
     * The windows will be registered with the Heaven::Application.
     *
     */
    PrimaryWindow::PrimaryWindow()
        : HeavenWindow( new PrimaryWindowPrivate, ViewIdentifier( UUIDSTR_PRIMARY_WINDOW ) )
    {
        HWPD(PrimaryWindow);

        if( Application::self()->primaryWindow() )
        {
            qFatal( "Only one PrimaryWindow is allowed at a time" );
        }

        d->owner = this;
        d->setup();

        setProperty( "heavenStyle", true );

        ApplicationPrivate::setPrimaryWindow( this );
    }

    /**
     * @brief       Destructor
     *
     */
    PrimaryWindow::~PrimaryWindow()
    {
        ApplicationPrivate::setPrimaryWindow( NULL );
    }

    /**
     * @brief       Get the menu bar
     *
     * @return      Pointer to the Heaven::MenuBar that was installed with setMenuBar().
     *
     */
    MenuBar* PrimaryWindow::menuBar() const
    {
        HWPD(const PrimaryWindow);
        return d->menuBar;
    }

    /**
     * @brief       Set or change the menubar
     *
     * @param[in]   bar     The Heaven::MenuBar to use for this PrimaryWindow.
     *
     */
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

    /**
     * @brief       Get the FooterWidget (status bar) for this PrimaryWindow
     *
     * @return      Pointer to the FooterWidget. If no FooterWidget exists, one will be created.
     *
     */
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

    /**
     * @brief       Set a FooterWidget
     *
     * @param[in]   bar     The footer widget to install.
     *
     * The PrimaryWindow takes ownership of the installed FooterWidget, any previously installed or
     * implicitly generated FooterWidget will be deleted (Including the widgets install onto it).
     *
     */
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

    /**
     * @brief       Is this a primary window?
     *
     * @return      Always `true`.
     *
     */
    bool PrimaryWindow::isPrimary() const
    {
        return true;
    }

}
