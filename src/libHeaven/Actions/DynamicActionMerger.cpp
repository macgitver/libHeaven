/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QWidget>

#include "Actions/DynamicActionMergerPrivate.hpp"

namespace Heaven
{

    DynamicActionMergerPrivate::DynamicActionMergerPrivate( DynamicActionMerger* owner )
        : UiObjectPrivate( owner )
    {
    }

    DynamicActionMergerPrivate::~DynamicActionMergerPrivate()
    {
    }

    UiObjectTypes DynamicActionMergerPrivate::type() const
    {
        return DynamicActionMergerType;
    }

    void DynamicActionMergerPrivate::addActionsTo( QWidget* widget )
    {
        static_cast< DynamicActionMerger* >( mOwner )->triggerRebuild();
        widget->addActions( mActions );
    }

    DynamicActionMerger::DynamicActionMerger( QObject* parent )
        : UiObject( parent, new DynamicActionMergerPrivate( this ) )
    {
    }

    void DynamicActionMerger::triggerRebuild()
    {
        UIOD(DynamicActionMerger);
        d->mActions.clear();
        QMetaObject::invokeMethod( parent(), d->mMergerSlot.constData(), Qt::DirectConnection,
                                   Q_ARG( Heaven::DynamicActionMerger*, this ) );
    }

    void DynamicActionMerger::setMergerSlot( const char *szSlot )
    {
        UIOD(DynamicActionMerger);
        d->mMergerSlot = szSlot;
    }

    void DynamicActionMerger::addAction( QAction* act )
    {
        UIOD(DynamicActionMerger);
        d->mActions.append( act );
    }

}
