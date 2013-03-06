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


    ViewContext::ViewContext()
        : d( new ViewContextPrivate( this ) )
    {
    }

    ViewContext::~ViewContext()
    {
        delete d;
    }

    ContextView* ViewContext::owningView()
    {
        return d->mOwningView;
    }

    void ViewContext::afterDetached()
    {
    }

    void ViewContext::beforeAttach()
    {
    }

}
