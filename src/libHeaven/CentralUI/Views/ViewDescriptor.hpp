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

#ifndef HEAVEN_VIEW_DESCRIPTOR_HPP
#define HEAVEN_VIEW_DESCRIPTOR_HPP

#include "libHeaven/HeavenApi.hpp"

#include "libHeaven/CentralUI/Views/ViewIdentifier.hpp"

namespace Heaven
{

    class View;

    class HEAVEN_API ViewDescriptor
    {
    public:
        typedef View* (*CreatorFunc)( );

    public:
        ViewDescriptor( const ViewIdentifier& id, const QString& displayName, CreatorFunc creator );

    public:
        QString displayName() const;
        ViewIdentifier identifier() const;
        ViewDescriptor::CreatorFunc creatorFunc() const;

        void unregister();
        View* createView();

    public:
        static ViewDescriptor* get( const ViewIdentifier& id );

    private:
        const ViewIdentifier    mIdentifier;
        const QString           mDisplayName;
        const CreatorFunc       mCreatorFunc;
        static QHash< ViewIdentifier, ViewDescriptor* > mDescriptors;
    };

}

#endif
