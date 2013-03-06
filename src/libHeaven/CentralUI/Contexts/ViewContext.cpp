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

#include "libHeaven/HeavenPrivate.hpp"

#include "libHeaven/CentralUI/Views/ContextView.hpp"

#include "libHeaven/CentralUI/Contexts/ViewContext.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextData.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextPrivate.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextManager.hpp"

namespace Heaven
{

    /**
     * @class   ViewContext
     * @ingroup CentralUI
     * @brief   A context that a view provides to other views
     *
     * A ContextView is able to provide and to consume contexts. A context represents the current
     * state of inter-dependent views.
     *
     * A master view is said to _provide_ a context and this context will automatically be attached
     * to slave views that have associated themselves with the master. They _consume_ the context.
     *
     * A _context consumer_ has the ability to store additional data into the context. This can be
     * achieved through ViewContextData objects.
     *
     * When a context and/or associated data is no longer in active use, it will be dropped after
     * a grace period.
     *
     * Contexts are identified by unique keys (ConextKeys objects). These keys always contain the
     * identifier of the ContextView to which they belong and optional as many key-value pairs as
     * required to make them unique.
     *
     * @par Providing contexts
     *
     * To create a ContextView provide contexts, a derivat of ContextView must set the flag
     * ProvidesContexts. Each such view that wants to provide a context must also implement a
     * derivat of ViewContext and return a new instance of it in the overloaded
     * ContextView::createContextObject() method.
     *
     * A pattern that can be followed for creation of new contexts is shown in the
     * ContextView::createContextObject() method documentation.
     *
     * @note    There are rare cases where it is not required to create a class inheriting from
     *          ViewContext. This is the case when all data can be stored into ViewContextData
     *          objects. However, in this scenario there is no direct call back for context
     *          switching. You can gain knowledge of the context change indirectly through the
     *          ViewContextData events, though.
     *
     * @par Consuming contexts
     *
     * To consume a context, one has to set the flag ConsumesContexts on the derivat of the
     * ContextView. The view must then know which contexts to attach to. This is done by giving the
     * identifier() of the providing view to setContextProvider().
     *
     * libHeaven will then internally call to ContextView::attachedContext() and
     * ContextView::detachedContext() as it is required. These method should be overloaded to
     * implement reactions to the context change in the providing view.
     *
     * Typical implementations are:
     * - For detachedContext(): Return the view to a state displaying nothing (i.e. like when the
     *   application was just started).
     * - For attachedContext(): Bring the view to display the state that is demanded by the context.
     *
     * @par Providing and consuming contexts at the same time
     *
     * It is possible to be consuming contexts and at the same time providing context to other
     * views. This way, it is possible to create a hierarchical data dependance.
     *
     *
     * @class   ViewContextPrivate
     * @internal
     * @ingroup CentralUI
     * @brief   Internal bridge between ViewContextManager and ViewContext
     *
     * Most of the implementation of ViewContext goes here. This class befriends the Manager and the
     * Context.
     *
     */

    ViewContextPrivate::ViewContextPrivate( ViewContext* owner )
        : mOwner( owner )
        , mOwningView( NULL )
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCP %p: Created", this );
        }
    }

    ViewContextPrivate::~ViewContextPrivate()
    {
        ViewContextManager::self().delContext( this );

        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCP %p: deleted", this );
        }
    }

    void ViewContextPrivate::setOwnerShip( ContextView* view )
    {
        if( view != mOwningView )
        {
            if( gDebugContexts )
            {
                qDebug( "VCP %p: Change owner from %p to %p", this, mOwningView, view );
            }

            mOwningView = view;

            if( !mOwningView )
            {
                ViewContextManager::self().updateExpireTime( this );
            }
        }
    }

    ContextView* ViewContextPrivate::ownerShip() const
    {
        return mOwningView;
    }

    void ViewContextPrivate::setKeys( const ContextKeys& keys )
    {
        mKeys = keys;

        if( gDebugContexts )
        {
            qDebug( "VCP %p: Keys => %s", this,
                    qPrintable( mKeys.toString() ) );
        }
    }

    const ContextKeys& ViewContextPrivate::keys() const
    {
        return mKeys;
    }

    ViewContext* ViewContextPrivate::owner()
    {
        return mOwner;
    }

    QDateTime ViewContextPrivate::expiresAt() const
    {
        return mExpiresAt;
    }

    void ViewContextPrivate::setExpireAt( const QDateTime& dt )
    {
        mExpiresAt = dt;
    }

    void ViewContextPrivate::expired()
    {
        if( gDebugContexts )
        {
            qDebug( "VCP %p: About to be expelled", this );
        }

        mOwner->deleteLater();
    }

    void ViewContextPrivate::setDataFor( const ViewIdentifier& id, ViewContextData* data )
    {
        ViewContextData* data2 = mDepData.take( id );

        if( data && data == data2 )
        {
            mDepData.insert( id, data );
            return;
        }

        if( data2 )
        {
            data2->setAttachedContext( NULL );
        }

        if( data )
        {
            data->setAttachedContext( this );
            mDepData.insert( id, data );
        }
    }

    ViewContextData* ViewContextPrivate::dataFor( const ViewIdentifier& id ) const
    {
        return mDepData.value( id, NULL );
    }

    void ViewContextPrivate::detach()
    {
        foreach( ContextView* view, ViewContextManager::self().dependantViews( mKeys.viewId() ) )
        {
            view->detachContext();
        }

        mOwner->afterDetached();
    }

    void ViewContextPrivate::attach()
    {
        mOwner->beforeAttach();

        foreach( ContextView* view, ViewContextManager::self().dependantViews( mKeys.viewId() ) )
        {
            view->attachContext( mOwner );
        }
    }


    /**
     * @brief       Constructor
     *
     * Internally creates the ViewContext.
     *
     */
    ViewContext::ViewContext()
        : d( new ViewContextPrivate( this ) )
    {
    }

    /**
     * @brief       Destructor
     */
    ViewContext::~ViewContext()
    {
        delete d;
    }

    /**
     * @brief       Get the owning view
     *
     * @return      A pointer to the owning ContextView.
     *
     * The owning view is the view that currently holds track of the context. It is set to `NULL`
     * in case this ContextView is closed.
     *
     * ViewContext objects without an owningView() don't cease to exist directly. A special grace
     * period for contexts that are both ownerless and unattached is granted.
     *
     * This is mainly useful in the case, the user shortly moves into another mode (i.e. Help) and
     * then returns to a mode that recreates the owningView().
     *
     * In common applications all view specific data would have been flushed out of memory in this
     * scenario and would have to be recreated just seconds later. libHeaven's grace periods make
     * such (silly) behaviour superfluous.
     *
     */
    ContextView* ViewContext::owningView()
    {
        return d->mOwningView;
    }

    /**
     * @brief       Callback to be invoked after detaching this context
     *
     * This virtual call back method is invoked directly after the context has internally been
     * detached from a ContextView (and thus is now in any grace period).
     *
     * This method should _only_ be used to drop memory with high cost that the implementation is
     * able to fastly regenerate.
     *
     */
    void ViewContext::afterDetached()
    {
    }

    /**
     * @brief       Callback to be invoked before attaching this context
     *
     * This virtual call back method is invoked before the context is internally attached to a view.
     *
     * It should _only_ be used to (re-)create local data that is fast to calculate but has high
     * memory cost.
     *
     */
    void ViewContext::beforeAttach()
    {
    }

}
