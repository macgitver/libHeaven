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

#ifndef HEAVEN_CONTEXT_VIEW_H
#define HEAVEN_CONTEXT_VIEW_H

#include <QWidget>

#include "libHeaven/CentralUI/Views/View.hpp"
#include "libHeaven/CentralUI/Contexts/ContextKeys.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContext.hpp"

namespace Heaven
{

    class ViewContextPrivate;
    class ViewContextManager;
    class ViewContextData;

    class HEAVEN_API ContextView : public View
    {
        Q_OBJECT
        friend class ViewContextPrivate;
        friend class ViewContextManager;
    public:
        ContextView( const ViewIdentifier& identifier );
        ~ContextView();

    public:
        enum Flag
        {
            ProvidesContexts    = 1 << 0,
            ConsumesContexts    = 1 << 1,
            DataPerContext      = 1 << 2
        };
        typedef QFlags< Flag > Flags;

    public:
        Flags flags() const;
        ViewContext* currentContext() const;

    protected:
        ContextKeys mkKeys();
        void setFlags( Flags flags );

    protected:  // for context consumers
        virtual ViewContextData* createContextData() const;
        virtual void attachedContext( ViewContext* ctx, ViewContextData* data );
        virtual void detachedContext( ViewContext* ctx );

        void setContextProvider( const ViewIdentifier& identifier );
        ViewIdentifier contextProvider() const;

    protected:  // for context providers
        virtual ViewContext* createContextObject() const;
        ViewContext* contextFor( const ContextKeys& keys, bool* isNewContext = NULL );
        void setCurrentContext( ViewContext* context );

    private:
        void attachContext( ViewContext* ctx );
        void detachContext();
        void updateAttachedContext( ViewContext* ctx, ViewContextData* data );

    private:
        Flags               mFlags;             //!< flags for this view
        ViewContext*        mCurrentContext;    //!< our current context
        ViewIdentifier      mProvider;          //!< the provider we depend on
        ViewContextData*    mCtxData;           //!< our data in the context we depend on
        ViewContext*        mAttachedContext;   //!< the context we're attached to
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS( Heaven::ContextView::Flags )

#endif
