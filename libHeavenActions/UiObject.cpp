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

#include "libHeavenActions/UiObject.hpp"
#include "libHeavenActions/UiObjectPrivate.hpp"
#include "libHeavenActions/UiContainer.hpp"
#include "libHeavenActions/UiManager.hpp"

namespace Heaven
{

    /**
     * @class   UiObject
     * @ingroup Actions
     * @brief   Base class for all Action-Framework objects
     *
     * All Action classes inherit this class. It provides the ability to be inserted into a
     * container and to have an activation context.
     *
     */

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

    /**
     * @brief       Constructor
     * @internal
     *
     * @param[in]   parent          Parent object in QObject hierarchy
     *
     * @param[in]   privateClass    Pointer to the private data container
     *
     */
    UiObject::UiObject( QObject* parent, UiObjectPrivate* privateClass )
        : QObject( parent )
        , mPrivate( privateClass )
    {
    }

    /**
     * @internal
     * @brief       Destructor
     */
    UiObject::~UiObject()
    {
        delete mPrivate;
    }

    /**
     * @brief       Set an activation context
     *
     * @param[in]   context     The context to set. This can be an arbitrary QObject. Ownership is
     *                          _not_ transfered.
     *
     * Whenever an action is triggered by the action framework, it internally walks the hierarchy
     * of (GUI-) objects upwards and tries to find an object with an activation context.
     *
     * Note, that hierarchy here does not relate to the document structure of the HID file. It is
     * the hierarchy of Gui objects.
     *
     */
    void UiObject::setActivationContext( QObject* context )
    {
        mPrivate->mActivationContext = context;
    }

    /**
     * @brief       Get the activation context
     *
     * @return      This object's activation context.
     *
     */
    QObject* UiObject::activationContext() const
    {
        return mPrivate->mActivationContext;
    }

    /**
     * @brief       Get the context that activated this object (action)
     *
     * @return      The activation context that was found in the hierarchy when the action was
     *              triggered.
     *
     */
    QObject* UiObject::activatedBy() const
    {
        return mPrivate->mActivatedBy;
    }

}
