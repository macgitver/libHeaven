
#include <QQueue>
#include <QStringBuilder>
#include <QDebug>

#include "HeavenPrivate.hpp"

#include "App/Application.hpp"
#include "App/ModeSwitcher.hpp"
#include "App/PrimaryWindow.hpp"

#include "Views/View.h"
#include "Views/ContainerWidgets/MultiBarContainerWidget.hpp"
#include "Views/ContainerWidgets/SplitterContainerWidget.hpp"

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

    ModeSwitcher::ModeSwitcher( Application* app, WindowStateRoot::Ptr state )
        : mApp( app )
        , mState( state )
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
     */
    void ModeSwitcher::synchronizeWindows()
    {
        foreach( const WindowStateBase::Ptr& ws, mState->children() )
        {
            Q_ASSERT( ws->type() == WindowStateBase::WSWindow );

            WindowStateWindow* windowState = static_cast< WindowStateWindow* >( ws.data() );
            QString windowId = windowState->identifier();

            HeavenWindow* window = mExistingWindows.take( windowId );
            if( !window )
            {
                window = mApp->window( windowId, true );
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
     */
    void ModeSwitcher::synchronizeWindow( HeavenWindow* window, WindowStateWindow* state )
    {
        Q_ASSERT( state->childrenCount() == 1 ); // A window may only have _one_ Container as child
                                                 // and may not be empty.

        WindowStateBase* child = state->childAt( 0 ).data();
        Q_ASSERT( child->type() != WindowStateBase::WSView
               && child->type() != WindowStateBase::WSWindow
               && child->type() != WindowStateBase::WSRoot );

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
     */
    void ModeSwitcher::synchronizeContainer( ContainerWidget* container, WindowStateBase* state )
    {
        QList< AbstractViewWidget* > newContents;

        foreach( const WindowStateBase::Ptr& ws, state->children() )
        {
            AbstractViewWidget* next = NULL;

            switch( ws->type() )
            {
            case WindowStateBase::WSView:
                next = grabView( static_cast< WindowStateView* >( ws.data() ) );
                break;

            case WindowStateBase::WSSplitter:
                next = grabSplitter( static_cast< WindowStateSplitter* >( ws.data() ) );
                synchronizeContainer( next->asContainerWidget(), ws.data() );
                break;

            case WindowStateBase::WSTab:
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

        // TODO: This is wrong here. Should not clear the container. This will deleteLater() all
        // views it contains; but we should preserve them.
        //container->clear();

        foreach( AbstractViewWidget* vcc, newContents )
        {
            container->add( vcc );
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

        QString viewId = view->identifier();

        View* realView = mExistingViews.take( viewId );
        if( realView )
        {
            ContainerWidget* parent = realView->parentContainer();
            parent->take( realView );
            return realView;
        }

        return mApp->createView( viewId );
    }

    /**
     * @brief       find or create a splitter
     *
     * @param[in]   splitter    The Splitter-State to look for
     *
     * @return      The found splitter or a newly created one.
     *
     * This code is not able to turn a MultiBar into a Splitter. It will assert in this case.
     *
     */
    AbstractViewWidget* ModeSwitcher::grabSplitter( WindowStateSplitter* splitter )
    {
        Q_ASSERT( splitter );
        QString id = splitter->identifier();

        SplitterContainerWidget* sc;
        sc = qobject_cast< SplitterContainerWidget* >( mExistingContainers.take( id ) );

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
     * @param[in]   splitter    The Tab-State to look for
     *
     * @return      The found splitter or a newly created one.
     *
     * This code is not able to turn a Splitter into a MultiBar. It will assert in this case.
     *
     */
    AbstractViewWidget* ModeSwitcher::grabTab( WindowStateTab* tab )
    {
        Q_ASSERT( tab );
        QString id = tab->identifier();

        MultiBarContainerWidget* mbw;
        mbw = qobject_cast< MultiBarContainerWidget* >( mExistingContainers.take( id ) );

        if( !mbw )
        {
            mbw = new MultiBarContainerWidget( id );
        }

        mbw->setBarPosition( tab->tabPosition() );
        return mbw;
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
    void ModeSwitcher::associateViewContainer( AbstractViewWidget* avw, WindowStateBase* ws )
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
     * Also populates the mOpened hash, which will probably soon become totally obsolete.
     */
    void ModeSwitcher::findOpened()
    {
        foreach( HeavenWindow* hw, mApp->allWindows() )
        {
            QString windowHandle = hw->handle();
            mExistingWindows.insert( windowHandle, hw );

            windowHandle += QChar( L'/' );

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
