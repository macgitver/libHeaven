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

namespace Heaven
{

    class ViewContext;
    class ContextView;

    class ViewContextPrivate
    {
        friend class ViewContext;
    public:
        ViewContextPrivate( ViewContext* owner );
        ~ViewContextPrivate();

    public:
        ViewContext* owner();

    public:
        /*
        void addRelatedContext( const QString& identifer, ViewContext* context );
        void removeRelatedContext( const QString& identifier );
        QStringList relatedContexts() const;
        ViewContext* getRelatedContext( const QString& identifier );
        */

    public:
        void setOwnerShip( ContextView* view );
        void setKeys( const ContextKeys& keys );
        const ContextKeys& keys() const;

        QDateTime expiresAt() const;
        void setExpireAt( const QDateTime& dt );

        void expired();

    public:
        static ViewContextPrivate* of( ViewContext* ctx ){ return ctx ? ctx->d : NULL; }

    private:
        ViewContext*    mOwner;
        ContextView*    mOwningView;
        ContextKeys     mKeys;
        QDateTime       mExpiresAt;
    };

    typedef QSet< ViewContextPrivate* > ViewContextPrivateSet;

}

#endif
