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
     * @ingroup     CentralUI
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
     * @enum        ContextView::Flag
     * @brief       A flag for Context Views
     *
     * @typedef     ContextView::Flags
     * @brief       A combination of Flag values
     *
     */

    /**
     * @brief       Constructor
     *
     * @param[in]   identifier  The identifier for this view.
     *
     */
    ContextView::ContextView( const ViewIdentifier& identifier )
        : View( identifier )
        , mCurrentContext( NULL )
        , mCtxData( NULL )
        , mAttachedContext( NULL )
    {
        ViewContextManager::self().viewOpened( this );
    }

    /**
     * @brief       Destructor
     *
     */
    ContextView::~ContextView()
    {
        ViewContextPrivateSet ctxs = ViewContextManager::self().contextsOwnedBy( this );

        foreach( ViewContextPrivate* vcp, ctxs )
        {
            vcp->setOwnerShip( NULL );
        }
        ViewContextManager::self().viewClosed( this );
    }

    /**
     * @brief       Set the current context of this view
     *
     * @param[in]   context     The context to switch to.
     *
     *
     *
     */
    void ContextView::setCurrentContext( ViewContext* context )
    {
        mCurrentContext = context;
        ViewContextManager::self().setCurrentContext( ViewContextPrivate::of( context ), this );
    }

    /**
     * @brief       Get the current context
     *
     * @return      The current context, or `NULL` if this view doesn't currently provide a context.
     *
     */
    ViewContext* ContextView::currentContext() const
    {
        return mCurrentContext;
    }

    /**
     * @brief       Get the attached context
     *
     * @return      The attached context, or `NULL` if the context provider does not have a
     *              current context.
     *
     */
    ViewContext* ContextView::attachedContext() const
    {
        return mAttachedContext;
    }

    /**
     * @brief       Get this view's flags
     *
     * @return      This view's current flags.
     *
     */
    ContextView::Flags ContextView::flags() const
    {
        return mFlags;
    }

    /**
     * @brief       Change this view's flags
     *
     * @param[in]   flags   The flags to change
     *
     * @param[in]   set     If `true`, the flags in @a flags will be set. If `false` the flags
     *                      given in @a flags will be unset.
     *
     */
    void ContextView::setFlags( ContextView::Flags flags, bool set )
    {
        if( set )
        {
            mFlags |= flags;
        }
        else
        {
            mFlags &= ~flags;
        }
    }

    /**
     * @brief       Create a default keys object for use by this view
     *
     * @return      A new, valid ContextKeys object associated with this view's identifier but no
     *              properties set.
     *
     */
    ContextKeys ContextView::mkKeys() const
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
     * @brief       Allocate a view specific ViewContext object
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

    void ContextView::attachedToContext( ViewContext* ctx, ViewContextData* data )
    {
    }

    void ContextView::detachedFromContext( ViewContext* ctx )
    {
    }

    /**
     * @internal
     * @brief       Attach to a context
     *
     * @param[in]   ctx The context to attach to
     *
     * This method is internally called to attach to a context.
     *
     * Depending on the flag DataPerContext, it will be tried to fetch this view's specific
     * ViewContextData object from the context. If the flag it is set and such a ViewContextData
     * object does not yet exist, it will be created (through a call to createContextData()) and
     * registered with the context.
     *
     * updateAttachedContext() is then called, which will take care of invoking the required call
     * backs.
     *
     */
    void ContextView::attachContext( ViewContext* ctx )
    {
        if( !ctx )
        {
            updateAttachedContext( NULL, NULL );
            return;
        }

        if( mFlags & DataPerContext )
        {
            ViewContextData* data = ViewContextPrivate::of( ctx )->dataFor( identifier() );
            if( !data )
            {
                data = createContextData();
                ViewContextPrivate::of( ctx )->setDataFor( identifier(), data );
            }

            updateAttachedContext( ctx, data );
        }
        else
        {
            updateAttachedContext( ctx, NULL );
        }
    }

    /**
     * @internal
     * @brief       Switch from no attached context to the given context
     *
     * @param[in]   ctx     The context to switch to
     *
     * @param[in]   data    Our own data, associated with the context.
     *
     * This method is internally called to do the actual update of the context and invoke the
     * attachedToContext() call back.
     *
     */
    void ContextView::updateAttachedContext( ViewContext* ctx, ViewContextData* data )
    {
        mAttachedContext = ctx;
        mCtxData = data;
        attachedToContext( ctx, data );
    }

    void ContextView::detachContext()
    {
        if( mAttachedContext )
        {
            if( mCtxData )
            {
            }
            detachedFromContext( mAttachedContext );
            mAttachedContext = NULL;
        }
    }

    /**
     * @brief       Set the context provider for this view
     *
     * @param[in]   identifier  A ViewIdentifier that names the ContextView, which will provide a
     *                          context to this view.
     *
     */
    void ContextView::setContextProvider( const ViewIdentifier& identifier )
    {
        if( mProvider != identifier )
        {
            detachContext();
            mProvider = identifier;

            ContextView* provView = ViewContextManager::self().viewFor( identifier );
            if( provView )
            {
                ViewContext* ctx = provView->currentContext();
                if( ctx )
                {
                    attachContext( ctx );
                }
            }
        }
    }

    /**
     * @brief       Get the identifier of the view that provides context to this view
     *
     * @return      The identifier of the view that provides this view's context.
     *
     */
    ViewIdentifier ContextView::contextProvider() const
    {
        return mProvider;
    }

    /**
     * @brief       Call back to create contextsensitive data for this view
     *
     * @return      A new object of a derivat of the ViewContextData class.
     *
     * You must overwrite this method, if you set the flag DataPerContext and if you do, you must
     * not return `NULL` from this method.
     *
     */
    ViewContextData* ContextView::createContextData() const
    {
        return NULL;
    }

}
