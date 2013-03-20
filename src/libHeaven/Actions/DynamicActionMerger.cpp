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

#include <QAction>
#include <QWidget>

#include "Actions/DynamicActionMergerPrivate.hpp"

namespace Heaven
{

    DynamicActionMergerPrivate::DynamicActionMergerPrivate( DynamicActionMerger* owner )
        : UiObjectPrivate( owner )
    {
        mMode = DAMergerCallback;
    }

    DynamicActionMergerPrivate::~DynamicActionMergerPrivate()
    {
        freeActionList();
    }

    void DynamicActionMergerPrivate::freeActionList()
    {
        foreach( ActionListEntry ale, mActions )
        {
            if( ale.mAction )
            {
                ale.mAction->disconnect( this );
            }

            if( ale.mLifetime == DAMergerActionMergerControlled )
            {
                delete ale.mAction;
            }
        }
        mActions.clear();
    }

    UiObjectTypes DynamicActionMergerPrivate::type() const
    {
        return DynamicActionMergerType;
    }

    void DynamicActionMergerPrivate::addActionsTo( QWidget* widget )
    {
        static_cast< DynamicActionMerger* >( mOwner )->triggerRebuild();

        foreach( ActionListEntry ale, mActions )
        {
            widget->addAction( ale.mAction );
        }
    }

    void DynamicActionMergerPrivate::onActionTriggered()
    {
        foreach( ActionListEntry le, mActions )
        {
            if( sender() == le.mAction )
            {
                DynamicActionMerger* dam = static_cast< DynamicActionMerger* >( mOwner );
                dam->entryTriggered( le.mValue );
                return;
            }
        }
    }

    DynamicActionMerger::DynamicActionMerger( QObject* parent )
        : UiObject( parent, new DynamicActionMergerPrivate( this ) )
    {
    }

    void DynamicActionMerger::triggerRebuild()
    {
        UIOD(DynamicActionMerger);

        if( d->mMode == DAMergerCallback )
        {
            d->mActions.clear();
            QMetaObject::invokeMethod( parent(), d->mMergerSlot.constData(), Qt::DirectConnection,
                                       Q_ARG( Heaven::DynamicActionMerger*, this ) );
        }
    }

    void DynamicActionMerger::setMergerSlot( const char *szSlot )
    {
        UIOD(DynamicActionMerger);
        d->mMergerSlot = szSlot;
    }

    void DynamicActionMerger::clear()
    {
        UIOD(DynamicActionMerger);
        d->freeActionList();
    }

    void DynamicActionMerger::addAction( QAction* act, const QVariant& value,
                                         MergerActionLifetime lifeTime )
    {
        UIOD(DynamicActionMerger);

        DynamicActionMergerPrivate::ActionListEntry le;
        le.mAction = act;
        le.mLifetime = lifeTime;
        le.mValue = value;

        if( act )
        {
            connect( act, SIGNAL(triggered()), d, SLOT(onActionTriggered()),
                     Qt::UniqueConnection );
        }

        d->mActions.append( le );
    }

    void DynamicActionMerger::addStringList( const QStringList& list )
    {
        foreach( QString s, list )
        {
            addAction( s, s );
        }
    }

    void DynamicActionMerger::addAction( const QString& display, const QVariant& value )
    {
        UIOD(DynamicActionMerger);

        DynamicActionMergerPrivate::ActionListEntry le;
        le.mAction = new QAction( display, this );
        le.mLifetime = DAMergerActionMergerControlled;
        le.mValue = value;

        if( le.mAction )
        {
            connect( le.mAction, SIGNAL(triggered()), d, SLOT(onActionTriggered()),
                     Qt::UniqueConnection );
        }

        d->mActions.append( le );
    }

    void DynamicActionMerger::addSeparator()
    {
        UIOD(DynamicActionMerger);

        DynamicActionMergerPrivate::ActionListEntry le;
        le.mAction = new QAction( this );
        le.mAction->setSeparator( true );
        le.mLifetime = DAMergerActionMergerControlled;
        d->mActions.append( le );
    }

    void DynamicActionMerger::setMode( MergerModes mode )
    {
        UIOD(DynamicActionMerger);
        d->mMode = mode;
    }

    MergerModes DynamicActionMerger::mode() const
    {
        UIOD(const DynamicActionMerger);
        return d->mMode;
    }

}
