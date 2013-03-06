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

#include "libHeaven/CentralUI/Contexts/ViewContextData.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextPrivate.hpp"

namespace Heaven
{

    ViewContextData::ViewContextData()
        : mAttachedContext( NULL )
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCD %p: Constructor", this );
        }
    }

    ViewContextData::~ViewContextData()
    {
        if( gDebugContexts && gDebugContextsVerbose )
        {
            qDebug( "VCD %p: Destructor", this );
        }

        // Assert that we're detached when being destructed.
        Q_ASSERT( !mAttachedContext );
    }

    void ViewContextData::setAttachedContext( ViewContextPrivate*  ctx )
    {
        if( mAttachedContext )
        {
            if( gDebugContexts )
            {
                qDebug( "VCD %p: detaching from context %p", this, mAttachedContext );
            }

            detachedFromContext();
            mAttachedContext = NULL;
        }

        if( ctx )
        {
            if( gDebugContexts )
            {
                qDebug( "VCD %p: attaching to context %p", this, ctx );
            }

            mAttachedContext = ctx;
            attachedToContext( ctx->owner() );
        }
    }

    ViewContext* ViewContextData::attachedContext() const
    {
        return mAttachedContext ? mAttachedContext->owner() : NULL;
    }

    void ViewContextData::detachedFromContext()
    {
    }

    void ViewContextData::attachedToContext( ViewContext* context )
    {
    }

}
