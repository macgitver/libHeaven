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

#ifndef HEAVEN_TOOLS_KVSTORE_HPP
#define HEAVEN_TOOLS_KVSTORE_HPP

#include <QSharedData>
#include <QVariant>

#include "libHeaven/HeavenApi.hpp"

namespace Heaven
{

    class HEAVEN_API KVStore
    {
    public:
        KVStore();
        KVStore( const KVStore& other );
        explicit KVStore( const QByteArray& name, const QVariant& value = QVariant() );
        ~KVStore();

    public:
        KVStore& operator=( const KVStore& other );
        KVStore& operator=( const QVariant& value );

        bool operator==( const KVStore& other ) const;
        bool operator==( const QByteArray& name ) const;
        bool operator!=( const KVStore& other ) const;
        bool operator!=( const QByteArray& name ) const;

        bool isValid() const;

    public:
        operator QVariant() const;
        operator QString() const;

        QByteArray name() const;
        QVariant value() const;

        bool hasKey( const QByteArray& name ) const;
        KVStore key( const QByteArray& name );
        KVStore operator[]( const QByteArray& name );

        QList< QByteArray > keys() const;

        void removeKey( const QByteArray& name );
        void clearKeys();
        KVStore copy() const;

    private:
        class Data;
        QExplicitlySharedDataPointer< Data > d;
    };

}

#endif
