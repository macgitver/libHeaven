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

#ifndef HEAVEN_VIEW_IDENTIFIER_HPP
#define HEAVEN_VIEW_IDENTIFIER_HPP

#include <QString>

#include "libHeaven/HeavenApi.hpp"

namespace Heaven
{

    class HEAVEN_API ViewIdentifier
    {
    public:
        ViewIdentifier();
        ViewIdentifier( const char* szName );
        ViewIdentifier( const ViewIdentifier& other );
        explicit ViewIdentifier( const QString& name );

    public:
        ViewIdentifier& operator=( const char* szName );
        ViewIdentifier& operator=( const ViewIdentifier& other );
        ViewIdentifier& operator=( const QString& name );

        bool operator==( const QString& other ) const;
        bool operator==( const ViewIdentifier& other ) const;
        bool operator!=( const QString& other ) const;
        bool operator!=( const ViewIdentifier& other ) const;

    public:
        QString toString() const;
        bool isValid() const;

    private:
        QString mName;
    };

    HEAVEN_API uint qHash( const ViewIdentifier& id );

    HEAVEN_API bool operator<(const ViewIdentifier& lhs, const ViewIdentifier& rhs);

}

HEAVEN_API QDebug& operator<<(QDebug& stream, const Heaven::ViewIdentifier& id);

#endif
