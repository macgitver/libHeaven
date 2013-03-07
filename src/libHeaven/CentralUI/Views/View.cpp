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

#include <QToolBar>
#include <QVBoxLayout>

#include "libHeaven/CentralUI/Views/View.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextManager.hpp"

namespace Heaven
{
    /**
     * @class   View
     * @ingroup CentralUI
     * @brief   Base class for content viewing widgets
     *
     * A view has a widget (set by setWidget()) which is displayed without border or margin inside
     * the view.
     * A view might have a ToolBar (set by setToolBar()), which is either displayed above the widget
     * or in the parentContainer().
     *
     * @property    View::viewName
     * @brief       The display name of this View.
     *
     * This is the (translated) visual name of the view. It appears i.e. on tab labels.
     *
     * @fn          View::nameChanged(const QString&)
     *
     * @param[in]   viewName    the new viewName for this view
     *
     * This signals is emitted when the translated display name of this view has changed.
     *
     * @fn          View::toolBarChanged(Heaven::ToolBar*)
     *
     * @param[in]   toolBar     the new tool bar of this view
     *
     * This signal is emitted when the view's tool bar has been changed (i.e. through a call to
     * setToolBar().
     *
     */

    /**
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier for this view. This is immutable after construction.
     *
     */
    View::View( const ViewIdentifier& identifier )
        : AbstractViewWidget( identifier )
        , mToolBar( NULL )
        , mRelayoutingIsQueued( false )
        , mRelayoutingForced( false )
        , mToolBarInOwnLayout( false )
    {
    }

    /**
     * @brief       Destructor
     */
    View::~View()
    {
    }

    /**
     * @brief       Get the view's display name
     *
     * @return      A translated string that can be presented to the user to refer to the view.
     *
     */
    QString View::viewName() const
    {
        return mViewName;
    }

    /**
     * @brief       Change the view's display name
     *
     * @param[in]   name    The new display name that should be used to present the user a reference
     *                      to this View.
     *
     * If the new name differs from the currently set one, the nameChanged() signal will be emitted.
     *
     */
    void View::setViewName( const QString& name )
    {
        if( name != mViewName )
        {
            mViewName = name;
            emit nameChanged( mViewName );
        }
    }

    void View::aboutToRemove()
    {
    }

    /**
     * @brief       Set this view's tool bar
     *
     * @param[in]   tb  A tool bar associated with this view or `NULL` if the view should not
     *                  display a tool bar at all.
     *
     * If the new tool bar is different from the currently set one, the toolBarChanged() singal will
     * be emitted.
     *
     * The layout of the View will be updated on the next return to Qt's event loop.
     *
     */
    void View::setToolBar( ToolBar* tb )
    {
        if( mToolBar != tb )
        {
            if( !mToolBar.isNull() && mToolBarInOwnLayout )
            {
                mToolBar->toolBarFor( this )->deleteLater();
            }

            mToolBar = tb;
            emit toolBarChanged( mToolBar );

            // If we already have a toolbar in our own layout, we _must_ relayout regardless of
            // the fact that the toolbar is still in our own layout.
            mRelayoutingForced |= mToolBarInOwnLayout;

            queueRelayouting();
        }
    }

    /**
     * @brief       Get this view's tool bar
     *
     * @return      A pointer to a Heaven::ToolBar that is associated with this view.
     *
     */
    ToolBar* View::toolBar() const
    {
        return mToolBar;
    }

    /**
     * @brief       Close this view
     *
     */
    void View::closeView()
    {
        parentContainer()->take( this );

        deleteLater();
    }

    /**
     * @brief       Change the view's widget.
     *
     * @param[in]   widget      The new widget to display inside this View or `NULL` to designate
     *                          the the view should not display a widget.
     *
     * The View takes ownership of the widget and will reparent the widget to itself.
     *
     * Any previously set QWidget will be deleted.
     *
     * The layout of the view will be updated on the next run of Qt's event loop.
     *
     */
    void View::setWidget( QWidget* widget )
    {
        if( widget != mWidget )
        {
            if( mWidget )
            {
                mWidget->deleteLater();
            }

            mWidget = widget;

            if( mWidget )
            {
                // Change the parent _now_. Don't wait until the event loop runs again.
                mWidget->hide();
                mWidget->setParent( this );
            }

            mRelayoutingForced = true;
            queueRelayouting();
        }
    }

    /**
     * @brief       Get the view's content widget
     *
     * @return      A QWidget pointer to the widget set by setWidget().
     *
     */
    QWidget* View::widget()
    {
        return mWidget;
    }

    /**
     * @internal
     * @brief       enqueue a relayouting onto the event loop
     *
     * If a relayouting is already enqueued, nothing will be done.
     *
     */
    void View::queueRelayouting()
    {
        if( !mRelayoutingIsQueued )
        {
            QMetaObject::invokeMethod( this, "performQueuedRelayouting", Qt::QueuedConnection );
            mRelayoutingIsQueued = true;
        }
    }

    /**
     * @internal
     * @brief       perform a queued relayouting
     *
     * Relayouts the view, if it is deemed to be required. It is required if mRelayoutingForced is
     * set to `true` or the type of the parentContainer() changed. In the latter case the position
     * of the toolbar has to be changed.
     *
     */
    void View::performQueuedRelayouting()
    {
        // We need an own toolBar, when there is a tool bar set and we have a parent container
        // which is NOT a MultiBarContainerWidget.
        bool needOwnToolBar = !mToolBar.isNull();
        needOwnToolBar &= parentContainer() &&
                          parentContainer()->containerType() != MultiBarContainerType;

        if( needOwnToolBar != mToolBarInOwnLayout )
        {
            // When toolbar or widget are changed, the internal code always sets relayouting
            // forced to true. But when we're comming from a MultiBarContainerWidget, we might
            // get a "add toolbar" followed by a "remove toolbar" (Moving from one MBC to another)
            // Which we catch by this weird logic... Which btw is the only real reason we need this
            // code to be queued.
            mRelayoutingForced = true;
        }

        if( mRelayoutingForced )
        {
            QVBoxLayout* l = new QVBoxLayout;
            l->setMargin( 0 );
            l->setSpacing( 0 );

            if( needOwnToolBar )
            {
                l->addWidget( mToolBar->toolBarFor( this ) );
            }
            if( !mWidget.isNull() )
            {
                l->addWidget( mWidget );

                if( !mWidget->isVisible() )
                {
                    mWidget->show();
                }
            }
            setLayout( l );

            mToolBarInOwnLayout = needOwnToolBar;
        }

        mRelayoutingForced = false;
        mRelayoutingIsQueued = false;
    }

}
