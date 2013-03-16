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

#include <QQueue>
#include <QStringBuilder>
#include <QDebug>

#include "HeavenPrivate.hpp"

#include "App/Application.hpp"
#include "App/ModeSwitcher.hpp"
#include "App/PrimaryWindow.hpp"

#include "CentralUI/Views/View.hpp"
#include "CentralUI/Views/ViewDescriptor.hpp"
#include "CentralUI/ContainerWidgets/MultiBarContainerWidget.hpp"
#include "CentralUI/ContainerWidgets/SplitterContainerWidget.hpp"

#include "libHeaven/CentralUI/States/WindowStateSplitter.hpp"
#include "libHeaven/CentralUI/States/WindowStateTab.hpp"
#include "libHeaven/CentralUI/States/WindowStateWindow.hpp"
#include "libHeaven/CentralUI/States/WindowStateView.hpp"

namespace Heaven
{

    /**
     * @internal
     * @class       ModeSwitcher
     * @brief       Handle switching modes
     *
     * The ModeSwitcher class handles all the gory details of switching a mode.
     *
     * After a pointer to the desired state is given through the constructor, the run() method
     * updates the current user interface to reflect the state as designated by the WindowStateRoot.
     *
     */

    /**
     * @brief       Constructor
     *
     * Setup a ModeSwitcher to synchronize the current CentralUI with a given WindowStateRoot.
     *
     * @param[in]   state   The WindowStateRoot to synchronize the current user interface to
     */
    ModeSwitcher::ModeSwitcher( WindowStateRoot::Ptr state )
        : mState( state )
    {
    }

    /**
     * @brief       Main work-horse.
     *
     */
    void ModeSwitcher::run()
    {
        findOpened();
        synchronizeWindows();
        killUnused();
    }

    /**
     * @brief       synchronize all windows
     *
     * Iterates over all windows that _shall be in the final state_ and matches them to existing
     * windows, creating new (secondary) windows on the fly.
     *
     * For each of those windows synchronzieWindow() is called, which will synchronize the window's
     * rootContainer().
     *
     */
    void ModeSwitcher::synchronizeWindows()
    {
        foreach( const WindowState::Ptr& ws, mState->children() )
        {
            Q_ASSERT( ws->type() == WindowState::WSWindow );

            WindowStateWindow* windowState = static_cast< WindowStateWindow* >( ws.data() );
            ViewIdentifier windowId = windowState->identifier();

            HeavenWindow* window = mExistingWindows.take( windowId );
            if( !window )
            {
                // this actually creates new windows (since we haven't found a window by that id
                // in the take() above the conditional.
                window = Heaven::app()->window( windowId, true );

                // TODO: new secondary windows should be made visible.
            }

            Q_ASSERT( window );
            synchronizeWindow( window, windowState );
        }
    }

    /**
     * @brief       synchronize a Top-Level HeavenWindow
     *
     * @param[in]   window      The window to synchronize
     *
     * @param[in]   state       The partial state to synch this window to.
     *
     * This method just asserts that the @a window is in the right shape and then calls out to
     * synchronize the rootContainer of the @a window.
     *
     */
    void ModeSwitcher::synchronizeWindow( HeavenWindow* window, WindowStateWindow* state )
    {
        Q_ASSERT( state->childrenCount() == 1 ); // A window may only have _one_ Container as child
                                                 // and may not be empty.

        WindowState* child = state->childAt( 0 ).data();
        Q_ASSERT( child->type() != WindowState::WSView
               && child->type() != WindowState::WSWindow
               && child->type() != WindowState::WSRoot );

        ContainerWidget* cw = window->rootContainer();
        synchronizeContainer( cw, child );
    }

    /**
     * @brief       synchronize a container
     *
     * @param[in]   container   The container to synchronize.
     *
     * @param[in]   state       The partial state to synch this container to.
     *
     * Synchronizes the container @a container and it's desired children to be as written down in
     * the WindowStateBase @a state.
     *
     * The process involves 3 steps:
     * 1.   find or create any View or ContainerWidget objects required to fill the container.
     *      This will recurse into sub containers.
     * 2.   remove anything that is left over in the @a container.
     * 3.   add the found objects to @a container.
     */
    void ModeSwitcher::synchronizeContainer( ContainerWidget* container, WindowState* state )
    {
        QList< AbstractViewWidget* > newContents;

        // We start by creating a list of what shall be inside the container. Thus we call the
        // grabXXX methods which will search anything existing and give it to us (in case it is
        // reusable). If grabXXX won't find anything, it will create something new.

        // During this run, we recurse into sub-containers.

        foreach( const WindowState::Ptr& ws, state->children() )
        {
            AbstractViewWidget* next = NULL;

            switch( ws->type() )
            {
            case WindowState::WSView:
                next = grabView( static_cast< WindowStateView* >( ws.data() ) );
                break;

            case WindowState::WSSplitter:
                next = grabSplitter( static_cast< WindowStateSplitter* >( ws.data() ) );
                synchronizeContainer( next->asContainerWidget(), ws.data() );
                break;

            case WindowState::WSTab:
                next = grabTab( static_cast< WindowStateTab* >( ws.data() ) );
                synchronizeContainer( next->asContainerWidget(), ws.data() );
                break;

            default:
                break;
            }

            Q_ASSERT( next );
            associateViewContainer( next, ws.data() );

            newContents.append( next );
        }

        // We have now taken all views that we want to reuse "out" of the container, leaving only
        // the ones which are referenced by mExistingViews but shall no longer be inside this
        // container. If they will not be reused until the end of this run, killUnsed() will take
        // care of conserving their content and delete the views.

        // So, all we need to do here is to take anything that's still inside the container away.

        while( container->count() )
        {
            container->takeAt( container->count() - 1 );
        }

        // Insert all Views/Containers we want into the ContainerWidget
        foreach( AbstractViewWidget* viewToAdd, newContents )
        {
            container->add( viewToAdd );
        }
    }

    /**
     * @brief       find or create a view
     *
     * @param[in]   view    The View-State to look for
     *
     * @return      The found view or a new view.
     *
     * When a view is reused, it is removed from the mExistingViews member.
     *
     */
    AbstractViewWidget* ModeSwitcher::grabView( WindowStateView* view )
    {
        Q_ASSERT( view );

        ViewIdentifier viewId = view->identifier();

        View* realView = mExistingViews.take( viewId );
        if( realView )
        {
            ContainerWidget* parent = realView->parentContainer();
            parent->take( realView );
            return realView;
        }

        ViewDescriptor* vd = ViewDescriptor::get( viewId );

        return vd ? vd->createView() : NULL;
    }

    /**
     * @brief       find or create a splitter
     *
     * @param[in]   splitter    The Splitter-State to look for
     *
     * @return      The found splitter or a newly created one.
     *
     * When the ContainerWidget with the required identifier is not a SplitterContainerWidget, it
     * will be replaced by a newly created one.
     *
     */
    AbstractViewWidget* ModeSwitcher::grabSplitter( WindowStateSplitter* splitter )
    {
        Q_ASSERT( splitter );
        ViewIdentifier id = splitter->identifier();

        SplitterContainerWidget* sc;

        ContainerWidget* cw = mExistingContainers.take( id );
        cleanUpContainer( SplitterContainerType, cw );

        sc = static_cast< SplitterContainerWidget* >( cw );
        if( !sc )
        {
            sc = new SplitterContainerWidget( id );
        }

        sc->setVertical( splitter->isVertical() );
        return sc;
    }

    /**
     * @brief       find or create a tab (MultiBar)
     *
     * @param[in]   tab     The Tab-State to look for
     *
     * @return      The found tab or a newly created one.
     *
     * When the ContainerWidget with the required identifier is not a MultiBarContainerWidget, it
     * will be replaced by a newly created one.
     *
     */
    AbstractViewWidget* ModeSwitcher::grabTab( WindowStateTab* tab )
    {
        Q_ASSERT( tab );
        ViewIdentifier id = tab->identifier();

        ContainerWidget* cw = mExistingContainers.take( id );
        cleanUpContainer( MultiBarContainerType, cw );

        MultiBarContainerWidget* mbw = static_cast< MultiBarContainerWidget* >( cw );
        if( !mbw )
        {
            mbw = new MultiBarContainerWidget( id );
        }

        mbw->setBarPosition( tab->tabPosition() );
        return mbw;
    }

    /**
     * @brief           clean up a container if it is not of a specific type
     *
     * @param[in]       unlessType  If the ContainerWidget @a cw is valid and of this type, nothing
     *                              will be done at all.
     *
     * @param[in,out]   cw          The ContainerWidget to clean up. This may be `NULL` on input, in
     *                              which case nothing is done. It will be set to `NULL` on output,
     *                              if the container was cleaned up (and deleted).
     *
     * This method is called to prohibit the usage of a SplitterContainerWidget when a
     * MultiBarContainerWidget is expected and vice versa. This can be the case if they have wrongly
     * the same identifier.
     *
     * When it is discovered that such a case is present, anything inside the container will be
     * taken away (It is still referenced in mExistingContainers and mExistingViews) and the
     * ContainerWidget @a cw will be deleted.
     *
     * It is assumed that @a cw is not in mExistingContainers.
     */
    void ModeSwitcher::cleanUpContainer( ContainerTypes unlessType, ContainerWidget*& cw )
    {
        if( !cw || cw->containerType() == unlessType )
        {
            return;
        }

        while( cw->count() )
        {
            cw->takeAt( cw->count() - 1 );
        }

        delete cw;
        cw = NULL;
    }

    /**
     * @brief       update association between a view/container and it's current state
     *
     * @param[in]   view    The view to update
     * @param[in]   wsView  The WindowState to update
     *
     * Views and WindowStateViews hold a non-counted reference to each other. This is used to call
     * into the view when the state is saved.
     *
     */
    void ModeSwitcher::associateViewContainer( AbstractViewWidget* avw, WindowState* ws )
    {
        // TODO: ws->setCurrentContent( vcc );
    }

    /**
     * @brief       find opened views, containers and windows
     *
     * Searches for currently opened views, containers and windows and poplates the mExistingXXX
     * members.
     *
     * As soon as an entry therein is reused, it must be removed from those hashes. Any view,
     * container or window still existing there will be deleted by killUnused() at the end of the
     * synchornization run.
     *
     */
    void ModeSwitcher::findOpened()
    {
        foreach( HeavenWindow* hw, Heaven::app()->allWindows() )
        {
            ViewIdentifier windowId = hw->handle();
            mExistingWindows.insert( windowId, hw );
            QQueue< ContainerWidget* > visit;

            visit.enqueue( hw->rootContainer() );

            while( !visit.isEmpty() )
            {
                ContainerWidget* cw = visit.dequeue();

                if( !cw )
                {
                    continue;
                }

                for( int i = 0; i < cw->count(); ++i )
                {
                    AbstractViewWidget* content = cw->widget( i );
                    if( content->isContainerWidget() )
                    {
                        ContainerWidget* child = content->asContainerWidget();
                        Q_ASSERT( child );

                        mExistingContainers.insert( content->identifier(), child );
                        visit.enqueue( child );
                    }
                    else
                    {
                        View* view = qobject_cast< View* >( content );
                        Q_ASSERT( view );

                        mExistingViews.insert( content->identifier(), view );
                    }
                }
            }
        }
    }

    /**
     * @brief       Delete any unused objects left over by the synchronization
     *
     */
    void ModeSwitcher::killUnused()
    {
        foreach( View* view, mExistingViews )
        {
            delete view;
        }

        foreach( AbstractViewWidget* avw, mExistingContainers )
        {
            delete avw;
        }

        foreach( HeavenWindow* hw, mExistingWindows )
        {
            delete hw;
        }
    }

 }
