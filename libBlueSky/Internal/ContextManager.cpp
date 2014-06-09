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

#include <QTimer>
#include <QDateTime>

#include "libBlueSky/Contexts.hpp"
#include "libBlueSky/Internal/ContextManager.hpp"

namespace BlueSky
{

    /**
     * @internal
     * @class   ViewContextManager
     * @ingroup CentralUI
     * @brief   Internal manager for Contexts and Views
     *
     * The ViewContextManager is the internal singleton implementation that glues various contexts
     * and views to each other.
     *
     * It manages a list of open ContextView objects, which contexts exist and in which state they
     * currently are.
     *
     * It is also responsible to trigger expiration internally. This is based on a timer with five
     * seconds accuracy.
     *
     */

    ViewContextManager::ViewContextManager()
    {
        mExpireTimer = new QTimer( this );
        mExpireTimer->setInterval( 1000 * 5 ); // every 5 seconds
        connect( mExpireTimer, SIGNAL(timeout()), this, SLOT(onContextExpireTimer()) );
        mExpireTimer->start();
    }

    ViewContextManager* ViewContextManager::sSelf = NULL;

    ViewContextManager& ViewContextManager::self()
    {
        if( !sSelf )
        {
            sSelf = new ViewContextManager();
        }
        return *sSelf;
    }

    void ViewContextManager::viewOpened( ContextView* view )
    {
        mOpenViews.insert( view->identifier(), view );
    }

    void ViewContextManager::viewClosed( ContextView* view )
    {
        mOpenViews.remove( view->identifier() );
    }

    ContextView* ViewContextManager::viewFor( const ViewIdentifier& id ) const
    {
        return mOpenViews.value( id, NULL );
    }

    ViewContextPrivateSet ViewContextManager::contextsOwnedBy( ContextView* view )
    {
        ViewContextPrivateSet ctxs;

        foreach( ViewContextPrivate* vcp, mContextsByKeys )
        {
            if( vcp->ownerShip() == view )
            {
                ctxs.insert( vcp );
            }
        }

        return ctxs;
    }

    /**
     * @brief       Add a new context
     *
     * @param[in]   ctx             The new context to be added.
     *
     * @param[in]   gracePeriod     The grace period for this newly created context. Actually, any
     *                              new context is not attached and thus up for deletion.
     *
     */
    void ViewContextManager::addContext( ViewContextPrivate* ctx , int gracePeriod )
    {
        mContextsByKeys.insert( ctx->keys(), ctx );
        mUnattachedContexts.insert( ctx );

        setContextToExpire( ctx, gracePeriod );
    }

    /**
     * @brief       Set an context to expire
     *
     * @param[in]   ctx     The context to set to expire
     *
     * @param[in]   gracePeriod The period in seconds to wait until the context will be expelled.
     *
     */
    void ViewContextManager::setContextToExpire( ViewContextPrivate* ctx, int gracePeriod )
    {
        if( gracePeriod )
        {
            QDateTime dt = QDateTime::currentDateTime().addSecs(gracePeriod);
            ctx->setExpireAt( dt );

            if( !mExpireContexts.contains( ctx ) )
            {
                mExpireContexts.insert( ctx );
            }

            if( gDebugContexts )
            {
                qDebug( "VCP %p: Set to expire in %ds at %s", ctx,
                        gracePeriod,
                        qPrintable( dt.toString( Qt::ISODate ) ) );
            }
        }
        else
        {
            if( gDebugContexts )
            {
                qDebug( "VCP %p: Set not to expire.", ctx );
            }
            if( mExpireContexts.contains( ctx ) )
            {
                mExpireContexts.remove( ctx );
            }
        }
    }

    void ViewContextManager::updateExpireTime( ViewContextPrivate* ctx )
    {
        int expiry = 0;

        if( !ctx->owner()->owningView() )
        {
            expiry = gGracePeriodContextShutdownOwnerless;
        }
        else if( mUnattachedContexts.contains( ctx ) )
        {
            expiry = gGracePeriodContextShutdownUnattached;
        }
        // TODO: Volunteered grace period

        setContextToExpire( ctx, expiry );
    }

    /**
     * @brief       Timer call back that actually expells contexts
     *
     */
    void ViewContextManager::onContextExpireTimer()
    {
        QDateTime now = QDateTime::currentDateTime();

        foreach( ViewContextPrivate* ctx, mExpireContexts )
        {
            QDateTime expires = ctx->expiresAt();

            if( now >= expires )
            {
                if( expires.isValid() )
                {
                    mExpireContexts.remove( ctx );
                }
                ctx->expired();
            }
        }
    }

    void ViewContextManager::delContext( ViewContextPrivate* ctx )
    {
        mAttachedContexts.remove( ctx );
        mUnattachedContexts.remove( ctx );
        mExpireContexts.remove( ctx );
        mContextsByKeys.remove( ctx->keys() );
    }

    /**
     * @brief       Find a context given its keys
     *
     * @param[in]   keys    The keys to search for.
     *
     * @return      A pointer to the ViewContextPrivate of the found context or `NULL` if no context
     *              for the @a keys was found.
     *
     */
    ViewContextPrivate* ViewContextManager::getContext( const ContextKeys& keys )
    {
        return mContextsByKeys.value( keys, NULL );
    }

    /**
     * @brief       Switch a view's context
     *
     * @param[in]   ctx     The new context to switch to.
     *
     * @param[in]   view    The view whose context is to be switched.
     *
     */
    void ViewContextManager::setCurrentContext( ViewContextPrivate* ctx, ContextView* view )
    {
        Q_ASSERT( view );

        ViewContextPrivate* oldContext = mCurrentViewContexts.take( view );
        if( gDebugContexts )
        {
            qDebug( "VM: Switching context for View %p[%s] from %p to %p",
                    view, qPrintable( view->identifier().toString() ), oldContext, ctx );
        }

        if( oldContext )
        {
            if( !mUnattachedContexts.contains( oldContext ) )
            {
                mUnattachedContexts.insert( oldContext );
            }
            if( mAttachedContexts.contains( oldContext ) )
            {
                mAttachedContexts.remove( oldContext );
            }

            oldContext->detach();

            updateExpireTime( oldContext );
        }

        if( ctx )
        {
            mCurrentViewContexts.insert( view, ctx );
            ctx->attach();

            if( !mAttachedContexts.contains( ctx ) )
            {
                mAttachedContexts.insert( ctx );
            }
            if( mUnattachedContexts.contains( ctx ) )
            {
                mUnattachedContexts.remove( ctx );
            }

            updateExpireTime( ctx );
        }
    }

    /**
     * @brief       Return a set of views that have a given context provider
     *
     * @param[in]   id  The identifier to look for.
     *
     * @return      A set of views whose contextProvider() is @a id.
     */
    QSet< ContextView* > ViewContextManager::dependantViews( const ViewIdentifier& id ) const
    {
        QSet< ContextView* > views;

        foreach( ContextView* view, mOpenViews )
        {
            if( view->flags() & ContextView::ConsumesContexts )
            {
                if( view->contextProvider() == id )
                {
                    views.insert( view );
                }
            }
        }

        return views;
    }
}
