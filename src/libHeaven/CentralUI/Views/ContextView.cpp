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

#include "libHeaven/HeavenPrivate.hpp"

#include "libHeaven/CentralUI/Views/ContextView.hpp"

#include "libHeaven/CentralUI/Contexts/ViewContextPrivate.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextManager.hpp"

namespace Heaven
{

    /**
     * @class       ContextView
     * @brief       Founding of contextual Views
     *
     * A ContextView is a View that provides or consumes contexts to or from other views.
     *
     * Three types of context views are possible:
     * - context views that provide contexts to other views.
     * - context views that consume context from other views.
     * - context views that do both of that at the same time.
     *
     * Each of these tasks has a different set of methods to support them.
     *
     */

    ContextView::ContextView( const ViewIdentifier& identifier )
        : View( identifier )
    {
    }

    ContextView::~ContextView()
    {
        ViewContextPrivateSet ctxs = ViewContextManager::self().contextsOwnedBy( this );

        foreach( ViewContextPrivate* vcp, ctxs )
        {
            vcp->setOwnerShip( NULL );
        }
    }

    ViewContext* ContextView::context()
    {
        return mCurrentContext;
    }

    void ContextView::setCurrentContext( ViewContext* context )
    {
        mCurrentContext = context;
        ViewContextManager::self().setCurrentContext( ViewContextPrivate::of( context ), this );
    }

    ContextView::Flags ContextView::flags() const
    {
        return mFlags;
    }

    void ContextView::setFlags( ContextView::Flags flags )
    {
        mFlags = flags;
    }

    /**
     * @brief       Create a default keys object for use by this view
     *
     * @return      A new, valid ContextKeys object associated with this view's identifier but no
     *              properties set.
     */
    ContextKeys ContextView::mkKeys()
    {
        ContextKeys keys( identifier() );
        return keys;
    }

    /**
     * @brief       Search or create a context for a given set of keys
     *
     * @param[in]       keys            The set of keys to look for.
     *
     * @param[in,out]   isNewContext    Pointer to a bool, which will receive whether the returned
     *                                  context was newly created and must be initialized or not.
     *                                  May be `NULL`.
     *
     * @return          A pointer to the found or newly created ViewContext. The owner of the
     *                  returned context is set to this View.
     *
     * If it is required to setup a new context, the virtual method createContextObject() is called
     * and the context is associated with the given @a keys.
     *
     * The returned context is not attached to a View, but ownership will still be given to the
     * creating View (`this`).
     *
     */
    ViewContext* ContextView::contextFor( const ContextKeys& keys, bool* isNewContext )
    {
        ViewContextPrivate* vcp = ViewContextManager::self().getContext( keys );

        if( !vcp )
        {
            if( isNewContext )
            {
                *isNewContext = true;
            }

            ViewContext* ctx = createContextObject();
            vcp = ViewContextPrivate::of( ctx );

            vcp->setKeys( keys );
            vcp->setOwnerShip( this );
            ViewContextManager::self().addContext( vcp, gGracePeriodContextShutdownUnattached );

            return ctx;
        }

        if( isNewContext )
        {
            *isNewContext = false;
        }

        vcp->setOwnerShip( this );

        return vcp->owner();
    }

    /**
     * @brief       Allocate a view specific ViewContex object
     *
     * @return      A ViewContext object suitable to store the context data of this view.
     *
     * Derivats must implement this method and return a valid, new ViewContext object. `NULL` must
     * _never_ be returned.
     *
     * Dependant views will get access to this object.
     *
     * The new object must not yet be initialized. If a View is able to store all it's data inside
     * the context keys, a simple `return new ViewContext;` is enough, which is what the default
     * implementation does.
     *
     * However, ViewContext objects can also be used as an anchor for hooking up interface
     * implementations based on the Q_INTERFACES() system.
     *
     */
    ViewContext* ContextView::createContextObject() const
    {
        return new ViewContext;
    }

    void ContextView::attachedContext( ViewContext* ctx )
    {
    }

    void ContextView::detachedContext( ViewContext* ctx )
    {
    }

    void ContextView::attachContext( ViewContext* ctx )
    {
        attachedContext( ctx );
    }

    void ContextView::detachContext( ViewContext* ctx )
    {
        detachedContext( ctx );
    }

    void ContextView::setContextProvider( const QString& identifier )
    {
        if( mProvider != identifier )
        {
            mProvider = identifier;
        }
    }

    /**
     * @brief       Get the identifier of the view that provides context to this view
     *
     * @return      The identifier of the view that provides this view's context.
     */
    QString ContextView::contextProvider() const
    {
        return mProvider;
    }

}
