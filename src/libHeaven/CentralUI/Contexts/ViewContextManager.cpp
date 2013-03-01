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

#include "libHeaven/HeavenPrivate.hpp"

#include "libHeaven/CentralUI/Contexts/ViewContextManager.hpp"

#include "libHeaven/CentralUI/Views/View.hpp"
#include "libHeaven/CentralUI/Views/ContextView.hpp"

namespace Heaven
{

    /**
     * @internal
     * @class   ViewContextManager
     * @ingroup CentralUI
     * @brief   Internal manager for Contexts and Views
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

    void ViewContextManager::viewOpened( View* view )
    {
        mOpenViews.insert( view->identifier(), view );
    }

    void ViewContextManager::viewClosed( View* view )
    {
        mOpenViews.remove( view->identifier() );
    }

    ViewContextPrivateSet ViewContextManager::contextsOwnedBy( ContextView* view )
    {
        return ViewContextPrivateSet();
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
        QDateTime dt = QDateTime::currentDateTime();
        dt.addSecs( gracePeriod );
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

        ViewContextPrivate* oldContext = mCurrentViewContexts.value( view, NULL );
        if( gDebugContexts )
        {
            qDebug( "VM: Switching context for View %p[%s] from %p to %p",
                    view, qPrintable( view->identifier() ), oldContext, ctx );
        }


    }

}
