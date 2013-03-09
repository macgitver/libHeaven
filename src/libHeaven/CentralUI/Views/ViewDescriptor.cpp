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

    ViewDescriptor::ViewDescriptor( const ViewIdentifier& id, const QString& displayName,
                                    ViewDescriptor::CreatorFunc creator )
        : mIdentifier( id  )
        , mDisplayName( displayName )
        , mCreatorFunc( creator )
    {
        mDescriptors.insert( id, this );
    }

    QHash< ViewIdentifier, ViewDescriptor* > ViewDescriptor::mDescriptors;

    QString ViewDescriptor::displayName() const
    {
        return mDisplayName;
    }

    ViewIdentifier ViewDescriptor::identifier() const
    {
        return mIdentifier;
    }

    ViewDescriptor::CreatorFunc ViewDescriptor::creatorFunc() const
    {
        return mCreatorFunc;
    }

    void ViewDescriptor::unregister()
    {
        mDescriptors.remove( mIdentifier );
        delete this;
    }

    ViewDescriptor* ViewDescriptor::get( const ViewIdentifier& id )
    {
        return mDescriptors.value( id, NULL );
    }

    View* ViewDescriptor::createView()
    {
        return mCreatorFunc();
    }


}
