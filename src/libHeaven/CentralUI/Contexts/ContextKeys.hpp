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

#ifndef HEAVEN_CONTEXT_KEYS_H
#define HEAVEN_CONTEXT_KEYS_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QSharedData>

#include "libHeaven/HeavenApi.hpp"

namespace Heaven
{

    class ViewIdentifier;

    class HEAVEN_API ContextKeys
    {
    public:
        ContextKeys();
        explicit ContextKeys( const ViewIdentifier& viewId );
        ContextKeys( const ContextKeys& other );
        ~ContextKeys();
        ContextKeys& operator=( const ContextKeys& other );
        bool isValid() const;

    public:
        bool operator==( const ContextKeys& other ) const;

    public:
        QString toString() const;
        ViewIdentifier viewId() const;

        QString operator[]( const char* pszKey ) const;
        QString operator[]( const QString& key ) const;

        QString get( const char* pszKey ) const;
        QString get( const QString& key ) const;

        void set( const char* pszKey, const QString& value );
        void set( const QString& key, const QString& value );

    private:
        class Data;
        QExplicitlySharedDataPointer< Data > d;
    };

    inline uint qHash( const ContextKeys& keys )
    {
        return qHash( keys.toString() );
    }

}

#endif
