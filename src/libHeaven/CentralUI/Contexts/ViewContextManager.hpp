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

#ifndef HEAVEN_VIEW_CONTEXT_MANAGER_H
#define HEAVEN_VIEW_CONTEXT_MANAGER_H

#include <QObject>
#include <QSet>
#include <QHash>

class QTimer;

#include "libHeaven/CentralUI/Views/ViewIdentifier.hpp"

#include "libHeaven/CentralUI/Contexts/ViewContext.hpp"
#include "libHeaven/CentralUI/Contexts/ViewContextPrivate.hpp"
#include "libHeaven/CentralUI/Contexts/ContextKeys.hpp"

namespace Heaven
{

    class View;
    class ViewContext;
    class ViewContextPrivate;
    class ContextKeys;

    class ViewContextManager : public QObject
    {
        Q_OBJECT
    private:
        ViewContextManager();
        static ViewContextManager* sSelf;

    public:
        static ViewContextManager& self();

    public:
        void viewOpened( ContextView* view );
        void viewClosed( ContextView* view );
        ContextView* viewFor( const ViewIdentifier& id ) const;
        QSet< ContextView* > dependantViews( const ViewIdentifier& id ) const;

        ViewContextPrivateSet contextsOwnedBy( ContextView* view );
        ViewContextPrivate* getContext( const ContextKeys& keys );
        void addContext( ViewContextPrivate* ctx, int gracePeriod );
        void delContext( ViewContextPrivate* ctx );

        void updateExpireTime( ViewContextPrivate* ctx );

        void setCurrentContext( ViewContextPrivate* ctx, ContextView* view );

    private slots:
        void onContextExpireTimer();

    private:
        void setContextToExpire( ViewContextPrivate* ctx, int gracePeriod );

    private:
        QHash< ViewIdentifier, ContextView* >       mOpenViews;

        QSet< ViewContextPrivate* >                 mAttachedContexts;
        QSet< ViewContextPrivate* >                 mUnattachedContexts;
        QSet< ViewContextPrivate* >                 mExpireContexts;
        QHash< ContextKeys, ViewContextPrivate* >   mContextsByKeys;
        QHash< ContextView*, ViewContextPrivate* >  mCurrentViewContexts;
        QTimer*                                     mExpireTimer;
    };

}

#endif
