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

#include "libHeaven/Actions/UiObject.hpp"
#include "libHeaven/Actions/UiObjectPrivate.hpp"
#include "libHeaven/Actions/UiContainer.h"
#include "libHeaven/Actions/UiManager.h"

namespace Heaven
{

    UiObjectPrivate::UiObjectPrivate( QObject* owner )
        : mActivationContext( NULL )
        , mActivatedBy( NULL )
        , mOwner( owner )
    {
        UiManager::self()->addUiObject( this );
    }

    UiObjectPrivate::~UiObjectPrivate()
    {
        foreach( UiContainer* container, mContainers )
        {
            removeFromContainer( container );
        }

        UiManager::self()->delUiObject( this );
    }

    void UiObjectPrivate::addedToContainer( UiContainer* container )
    {
        mContainers.insert( container );
    }

    void UiObjectPrivate::removeFromContainer( UiContainer* container )
    {
        container->remove( this );
    }

    void UiObjectPrivate::removedFromContainer( UiContainer* container )
    {
        mContainers.remove( container );
    }

    void UiObjectPrivate::findActivationContext( QObject* trigger )
    {
        mActivatedBy = UiManager::self()->findActivationContext( trigger );
    }

    UiObject::UiObject( QObject* parent, UiObjectPrivate* privateClass )
        : QObject( parent )
        , mPrivate( privateClass )
    {
    }

    UiObject::~UiObject()
    {
        delete mPrivate;
    }

    void UiObject::setActivationContext( QObject* context )
    {
        mPrivate->mActivationContext = context;
    }

    QObject* UiObject::activationContext() const
    {
        return mPrivate->mActivationContext;
    }

    QObject* UiObject::activatedBy() const
    {
        return mPrivate->mActivatedBy;
    }

}
