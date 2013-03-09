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

#include "libHeaven/CentralUI/Views/ViewDescriptor.hpp"

namespace Heaven
{

    /**
     * @class   ViewDescriptor
     * @ingroup CentralUI
     * @brief   A descriptor for available views
     *
     * This class stores meta data about the available views. It alos acts as a factory to create
     * real View objects, when required.
     *
     * Each instance of this class represents one View.
     *
     * @typedef ViewDescriptor::CreatorFunc
     * @brief   Pointer to a creator function that creates a view
     *
     */

    /**
     * @brief   Constructor
     *
     * @param[in]   id              The identifier to register.
     *
     * @param[in]   displayName     The translated text to display when referring to this View.
     *
     * @param[in]   creator         Creator function that is able to create this type of View
     *                              object.
     *
     * The descriptor must be created on the __heap__. It will be automatically registered.
     *
     */
    ViewDescriptor::ViewDescriptor( const ViewIdentifier& id, const QString& displayName,
                                    ViewDescriptor::CreatorFunc creator )
        : mIdentifier( id  )
        , mDisplayName( displayName )
        , mCreatorFunc( creator )
    {
        mDescriptors.insert( id, this );
    }

    QHash< ViewIdentifier, ViewDescriptor* > ViewDescriptor::mDescriptors;

    /**
     * @brief       Get the display name for this descriptor
     *
     * @return      A translated text suitable to refer to the View.
     *
     */
    QString ViewDescriptor::displayName() const
    {
        return mDisplayName;
    }

    /**
     * @brief       Get the identifier for this descriptor
     *
     * @return      The identifier for this descriptor.
     *
     */
    ViewIdentifier ViewDescriptor::identifier() const
    {
        return mIdentifier;
    }

    /**
     * @brief       Get the creator function for this descriptor
     *
     * @return      The creator function for this descriptor
     *
     */
    ViewDescriptor::CreatorFunc ViewDescriptor::creatorFunc() const
    {
        return mCreatorFunc;
    }

    /**
     * @brief       Unregisters this descriptor and deletes it
     *
     */
    void ViewDescriptor::unregister()
    {
        mDescriptors.remove( mIdentifier );
        delete this;
    }

    /**
     * @brief       Lookup a descriptor by its identifier
     *
     * @param[in]   id  The identifier to look up.
     *
     * @return      Pointer to the found ViewDescriptor or `NULL` if no descriptor was found.
     *
     */
    ViewDescriptor* ViewDescriptor::get( const ViewIdentifier& id )
    {
        return mDescriptors.value( id, NULL );
    }

    /**
     * @brief       Create a new view
     *
     * @return      Pointer to the new View.
     *
     * Invokes the creator function and forwards its return value.
     *
     */
    View* ViewDescriptor::createView() const
    {
        return mCreatorFunc();
    }


}
