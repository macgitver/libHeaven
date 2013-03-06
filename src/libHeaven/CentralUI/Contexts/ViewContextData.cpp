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

    /**
     * @brief       Get the attached context, if any
     *
     * @return      A pointer to the attached ViewContext or `NULL` if this ViewContextData is not
     *              attached to a ViewContext.
     *
     */
    ViewContext* ViewContextData::attachedContext() const
    {
        return mAttachedContext ? mAttachedContext->owner() : NULL;
    }

    /**
     * @brief       Callback on detaching from a context
     *
     * This virtual call back is called directly _before_ detaching this ViewContextData from a
     * ViewContext object. In theory, this should happen only once, right before the destruction.
     *
     * While this call back is invoked, attachedContext() still returns a reference to the context.
     * However, you should assume that it is a ViewContext object and _not a derivat of that_, since
     * this method might be implicitly called from the view context's destructor.
     *
     */
    void ViewContextData::detachedFromContext()
    {
    }

    /**
     * @brief       Callback on attaching to a context
     *
     * @param[in]   context     The context that this data was just attached to.
     *
     * This virtual call back is called directly after attaching this ViewContextData to a
     * ViewContext object. In theory, this should happen only once.
     *
     */
    void ViewContextData::attachedToContext( ViewContext* context )
    {
    }

}
