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

#include <QWidget>

#include "libHeavenActions/UiManager.hpp"
#include "libHeavenActions/UiContainer.hpp"

namespace Heaven
{

    UiManager::UiManager()
        : QObject()
    {
    }

    UiManager::~UiManager()
    {
    }

    UiManager* UiManager::sSelf = NULL;

    UiManager* UiManager::self()
    {
        if( sSelf == NULL )
        {
            sSelf = new UiManager;
        }

        return sSelf;
    }

    void UiManager::addUiObject( UiObjectPrivate* uio )
    {
        mUioUsage.insert( uio, QSet< UiObjectPrivate* >() );
    }

    void UiManager::delUiObject( UiObjectPrivate* uio )
    {
        QSet< UiObjectPrivate* > usage = mUioUsage.value( uio );
        mUioUsage.remove( uio );

        foreach( UiObjectPrivate* used, usage )
        {
            Q_UNUSED( used );
        }
    }

    void UiManager::addCreatedObject( QObject* object, UiObjectPrivate* forUiObject )
    {
        mCreatedObjects.insert( object, forUiObject );
    }

    void UiManager::removeCreatedObject( QObject* object )
    {
        mCreatedObjects.remove( object );
    }

    QObject* UiManager::findActivationContext( QObject* trigger )
    {
        UiObjectPrivate* previous;
        UiObjectPrivate* uiObject = NULL;
        Q_ASSERT( trigger );

        do
        {
            previous = uiObject;
            uiObject = mCreatedObjects.value( trigger, NULL );
            if( !uiObject )
            {
                return NULL;
            }

            UiObjectTypes type = uiObject->type();
            if( type == ToolBarType || type == MenuType || type == MenuBarType )
            {
                UiContainer* container = static_cast< UiContainer* >( uiObject );
                QList< UiContainer* > pathTo = container->pathTo( previous );
                while( !pathTo.isEmpty() )
                {
                    UiContainer* cur = pathTo.takeLast();
                    if( cur->mActivationContext )
                    {
                        return cur->mActivationContext;
                    }
                }
            }

            if( uiObject->mActivationContext )
            {
                return uiObject->mActivationContext;
            }

            trigger = trigger->parent();
        } while( trigger );

        return NULL;
    }

}
