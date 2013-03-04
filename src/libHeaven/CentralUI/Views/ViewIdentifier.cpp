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

#include "libHeaven/CentralUI/Views/ViewIdentifier.hpp"

namespace Heaven
{

    ViewIdentifier::ViewIdentifier()
    {
    }

    ViewIdentifier::ViewIdentifier( const char* szName )
        : mName( QLatin1String( szName ) )
    {
    }

    ViewIdentifier::ViewIdentifier( const ViewIdentifier& other )
        : mName( other.mName )
    {
    }

    ViewIdentifier::ViewIdentifier( const QString& name )
        : mName( name )
    {
    }

    ViewIdentifier& ViewIdentifier::operator=( const char* szName )
    {
        Q_ASSERT( szName );
        mName = QLatin1String( szName );
        return *this;
    }

    ViewIdentifier& ViewIdentifier::operator=( const ViewIdentifier& other )
    {
        mName = other.mName;
        return *this;
    }

    ViewIdentifier& ViewIdentifier::operator=( const QString& name )
    {
        mName = name;
        return *this;
    }

    bool ViewIdentifier::operator==( const QString& other ) const
    {
        return mName == other;
    }

    bool ViewIdentifier::operator==( const ViewIdentifier& other ) const
    {
        return mName == other.mName;
    }

    QString ViewIdentifier::toString() const
    {
        return mName;
    }

    bool ViewIdentifier::isValid() const
    {
        return !mName.isEmpty();
    }

}
