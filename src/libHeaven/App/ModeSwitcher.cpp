
#include <QQueue>
#include <QStringBuilder>
#include <QDebug>

#include "HeavenPrivate.hpp"

#include "App/Application.hpp"
#include "App/ModeSwitcher.hpp"
#include "App/PrimaryWindow.hpp"

#include "Views/View.h"
#include "Views/TopLevelWidget.h"

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

        ViewContainer* vc = window->topLevelContainer()->rootContainer();
        synchronizeContainer( vc, child );
    }

    /**
     * @brief       synchronize a container
     *
     * @param[in]   container   The container to synchronize.
     *
     * @param[in]   state       The partial state to synch this container to.
     *
     */
    void ModeSwitcher::synchronizeContainer( ViewContainer* container, WindowStateBase* state )
    {
        QList< ViewContainerContent* > newContents;

        foreach( const WindowStateBase::Ptr& ws, state->children() )
        {
            ViewContainerContent* next = NULL;

            switch( ws->type() )
            {
            case WindowStateBase::WSView:
                next = grabView( static_cast< WindowStateView* >( ws.data() ) );
                break;

            case WindowStateBase::WSSplitter:
                next = grabSplitter( static_cast< WindowStateSplitter* >( ws.data() ) );
                synchronizeContainer( next->asContainer(), ws.data() );
                break;

            case WindowStateBase::WSTab:
                next = grabTab( static_cast< WindowStateTab* >( ws.data() ) );
                synchronizeContainer( next->asContainer(), ws.data() );
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
        container->clear();

        foreach( ViewContainerContent* vcc, newContents )
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
    ViewContainerContent* ModeSwitcher::grabView( WindowStateView* view )
    {
        Q_ASSERT( view );

        QString viewId = view->identifier();

        View* realView = mExistingViews.take( viewId );
        if( realView )
        {
            ViewContainer* parent = realView->container();
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
    ViewContainerContent* ModeSwitcher::grabSplitter( WindowStateSplitter* splitter )
    {
        Q_ASSERT( splitter );

        QString id = splitter->identifier();

        ViewContainerContent* vcc = mExistingContainers.take( id );
        if( vcc )
        {
            ViewContainer* vc = vcc->asContainer();
            Q_ASSERT( vc ); // Must be true, since mExistingContainers only contains containers :-)
            Q_ASSERT( vc->type() == ViewContainer::Splitter );

            bool isVert = vc->subtype() == ViewContainer::SubSplitVert;
            if( splitter->isVertical() != isVert )
            {
                vc->changeSubtype( splitter->isVertical() ? ViewContainer::SubSplitVert
                                                          : ViewContainer::SubSplitHorz );
            }

            return vcc;
        }

        ViewContainer* vc = new ViewContainer( id, ViewContainer::Splitter,
                                               splitter->isVertical() ? ViewContainer::SubSplitVert
                                                                      : ViewContainer::SubSplitHorz );
        return vc;
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
    ViewContainerContent* ModeSwitcher::grabTab( WindowStateTab* tab )
    {
        Q_ASSERT( tab );

        QString id = tab->identifier();
        ViewContainer::Subtype subType = tab->subtype();

        ViewContainerContent* vcc = mExistingContainers.take( id );
        if( vcc )
        {
            ViewContainer* vc = vcc->asContainer();
            Q_ASSERT( vc ); // Must be true, since mExistingContainers only contains containers :-)
            Q_ASSERT( vc->type() == ViewContainer::MultiBar );

            if( vc->subtype() != subType )
            {
                vc->changeSubtype( subType );
            }
            return vcc;
        }

        ViewContainer* vc = new ViewContainer( id, ViewContainer::MultiBar, subType );
        return vc;
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
    void ModeSwitcher::associateViewContainer( ViewContainerContent* vcc, WindowStateBase* ws )
    {
        ws->setCurrentContent( vcc );
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

            mOpened.append( windowHandle );
            windowHandle += QChar( L'/' );

            QQueue< ViewContainer* > visit;

            visit.enqueue( hw->topLevelContainer()->rootContainer() );

            while( !visit.isEmpty() )
            {
                ViewContainer* vc = visit.dequeue();

                if( !vc )
                {
                    continue;
                }

                mOpened.append( windowHandle % vc->handle() );

                foreach( ViewContainerContent* cc, vc->contents() )
                {
                    if( cc->isContainer() )
                    {
                        mExistingContainers.insert( cc->identifier(), cc );
                        visit.enqueue( cc->asContainer() );
                    }
                    else
                    {
                        mOpened.append( windowHandle % cc->handle() );
                        mExistingViews.insert( cc->identifier(), cc->asView() );
                    }
                }
            }
        }

        qDebug() << "Opened=" << mOpened;
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

        foreach( ViewContainerContent* vcc, mExistingContainers )
        {
            delete vcc;
        }

        foreach( HeavenWindow* hw, mExistingWindows )
        {
            delete hw;
        }
    }

 }
