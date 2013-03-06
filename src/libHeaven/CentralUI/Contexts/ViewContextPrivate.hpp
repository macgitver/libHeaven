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

#ifndef HEAVEN_VIEW_CONTEXT_PRIVATE_HPP
#define HEAVEN_VIEW_CONTEXT_PRIVATE_HPP

#include <QDateTime>
#include <QString>
#include <QSet>

#include "libHeaven/CentralUI/Contexts/ContextKeys.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContext.hpp"

namespace Heaven
{

    class ViewContext;
    class ContextView;
    class ViewContextData;

    class ViewContextPrivate
    {
        friend class ViewContext;
    public:
        ViewContextPrivate( ViewContext* owner );
        ~ViewContextPrivate();

    public:
        ViewContext* owner();

    public:
        void setOwnerShip( ContextView* view );
        void setKeys( const ContextKeys& keys );
        const ContextKeys& keys() const;

        QDateTime expiresAt() const;
        void setExpireAt( const QDateTime& dt );

        void expired();

    public:
        void setDataFor( const ViewIdentifier& id, ViewContextData* data );
        ViewContextData* dataFor( const ViewIdentifier& id ) const;

    public:
        void detach();
        void attach();

    public:
        static ViewContextPrivate* of( ViewContext* ctx ){ return ctx ? ctx->d : NULL; }

    private:
        ViewContext*    mOwner;
        ContextView*    mOwningView;
        ContextKeys     mKeys;
        QDateTime       mExpiresAt;
        QHash< ViewIdentifier, ViewContextData* > mDepData;
    };

    typedef QSet< ViewContextPrivate* > ViewContextPrivateSet;

}

#endif
