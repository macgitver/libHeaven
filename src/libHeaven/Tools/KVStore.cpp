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

#include <QHash>

#include "libHeaven/Tools/KVStore.hpp"

namespace Heaven
{

    /**
     * @class   KVStore
     * @ingroup HeavenTools
     * @brief   Generic purpose hierarchical Key-Value store
     *
     * This is a Key Value store that is generic in purpose. It is hierarchical. Each hierarchy
     * level manages itself and its children; there is no manager required. Thus, each hierarchy
     * level can act as a root on its own.
     *
     * A KVStore is created by calling the constructor with a name and an optional value. It should
     * not be allocated on the heap; a KVStore can be used as a value class.
     *
     *
     */

    class KVStore::Data : public QSharedData
    {
    public:
        Data(){}
        Data( const Data& other )
        {
            name = other.name;
            value = other.value;

            foreach( const KVStore& s, other.children )
            {
                children.insert( s.name(), s.copy() );
            }
        }

    public:
        QByteArray name;
        QVariant value;
        QHash< QByteArray, KVStore > children;
    };

    /**
     * @brief       Constructor (Default)
     *
     * Creates an invalid KVStore that cannot be assigned a value to.
     *
     */
    KVStore::KVStore()
    {
    }

    /**
     * @brief       Constructor (Copy)
     *
     * @param[in]   other   The KVStore to create a copy of.
     *
     * Creates a KVStore that will access the same data as @a other. You can use copy() to create
     * a deep copy.
     *
     */
    KVStore::KVStore( const KVStore& other )
        : d( other.d )
    {
    }

    /**
     * @brief       Constructor (Create)
     *
     * @param[in]   name    The name of this KVStore. This is immutable after construction.
     *
     * @param[in]   value   An optional initial value of this KVStore.
     *
     */
    KVStore::KVStore( const QByteArray& name, const QVariant& value )
    {
        d = new Data;
        d->name = name;
        d->value = value;
    }

    /**
     * @brief       Destructor
     */
    KVStore::~KVStore()
    {
    }

    /**
     * @brief       Assignment operator
     *
     * @param[in]   other   The other KVStore to assign
     *
     * @return      A reference to this KVStore.
     *
     * The two KVStores will share the same data; a change in one is visible in the other as well.
     * To create a deep copy, use the copy() method instead.
     */
    KVStore& KVStore::operator=( const KVStore& other )
    {
        d = other.d;
        return *this;
    }

    /**
     * @brief       Set this store's value
     *
     * @param[in]   value   The value to set.
     *
     * @return      A reference to this KVStore.
     *
     */
    KVStore& KVStore::operator=( const QVariant& value )
    {
        Q_ASSERT( isValid() );
        d->value = value;
        return *this;
    }

    /**
     * @brief       Compare operator
     *
     * @param[in]   other   The other KVStore to compare against
     *
     * @return      `true`, if both KVStores are equal (share the same data). `false` otherwise.
     *
     */
    bool KVStore::operator==( const KVStore& other ) const
    {
        return d == other.d;
    }

    /**
     * @brief       Compare operator (key name)
     *
     * @param[in]   name    the name to compare the key name against.
     *
     * @return      `true` if the name of this KVStore is @a name, `false` otherwise.
     *
     */
    bool KVStore::operator==( const QByteArray& name ) const
    {
        return isValid() && d->name == name;
    }

    /**
     * @brief       Compare operator
     *
     * @param[in]   other   The other KVStore to compare against
     *
     * @return      `false`, if both KVStores are equal (share the same data). `true` otherwise.
     *
     */
    bool KVStore::operator!=( const KVStore& other ) const
    {
        return d != other.d;
    }

    /**
     * @brief       Compare operator (key name)
     *
     * @param[in]   name    the name to compare the key name against.
     *
     * @return      `true` if the name of this KVStore is __not__ @a name, `false` otherwise.
     *
     */
    bool KVStore::operator!=( const QByteArray& name ) const
    {
        return !isValid() || d->name != name;
    }

    /**
     * @brief       Check validity of this KVStore
     *
     * @return      `true` if this KVStore is valid, `false` otherwise.
     *
     */
    bool KVStore::isValid() const
    {
        return !!d;
    }

    /**
     * @brief       Get this KVStore's value
     *
     */
    KVStore::operator QVariant() const
    {
        return isValid() ? d->value : QVariant();
    }

    /**
     * @brief       Get this KVStore's value
     *
     */
    KVStore::operator QString() const
    {
        return isValid() ? d->value.toString() : QString();
    }

    /**
     * @brief       Get this KVStore's name
     *
     * @return      If this KVStore is valid, return its name - otherwise an empty text.
     *
     */
    QByteArray KVStore::name() const
    {
        return isValid() ? d->name : QByteArray();
    }

    /**
     * @brief       Get this KVStore's value
     *
     * @return      The value or a default constructed QVariant if this KVStore is not valid.
     *
     */
    QVariant KVStore::value() const
    {
        return isValid() ? d->value : QVariant();
    }

    /**
     * @brief       Query existance of a key
     *
     * @param[in]   name    The name of the child key to query for
     *
     * @return      `true` if it exists and `false` if not or this KVStore itself ain't valid.
     *
     */
    bool KVStore::hasKey( const QByteArray& name ) const
    {
        return isValid() && d->children.contains( name );
    }

    /**
     * @brief       Find or create a key
     *
     * @param[in]   name    The key to get
     *
     * @return      The child key named @a name
     *
     * Creates the key if it doesn't exist yet.
     *
     */
    KVStore KVStore::key( const QByteArray& name )
    {
        if( isValid() )
        {
            if( d->children.contains( name ) )
            {
                return d->children.value( name );
            }

            KVStore key( name );
            d->children.insert( name, key );
            return key;
        }

        return KVStore();
    }

    /**
     * @brief       Index operator
     *
     * @param[in]   name    The key to get
     *
     * @return      The child key named @a name
     *
     * Creates the key if it doesn't exist yet.
     *
     */
    KVStore KVStore::operator[]( const QByteArray& name )
    {
        return key( name );
    }

    /**
     * @brief       Get a list of all children
     *
     * @return      A list of all children's key names if this is a valid KVStore; an empty list
     *              otherwise.
     *
     */
    QList< QByteArray > KVStore::keys() const
    {
        return isValid() ? d->children.keys() : QList< QByteArray >();
    }

    /**
     * @brief       Remove a child key from this KVStore
     *
     * @param[in]   name    the name of the child key to remove
     *
     * Does nothing if this is not a valid KVStore.
     */
    void KVStore::removeKey( const QByteArray& name )
    {
        if( isValid() )
        {
            d->children.remove( name );
        }
    }

    /**
     * @brief       clear all children
     *
     * Removes all children from this KVStore.
     *
     * Does nothing if this is not a valid KVStore.
     *
     */
    void KVStore::clearKeys()
    {
        if( isValid() )
        {
            d->children.clear();
        }
    }

    /**
     * @brief       Create a deep copy
     *
     * @return      A KVStore that will not share any current data with this store.
     *
     */
    KVStore KVStore::copy() const
    {
        KVStore s;

        if( isValid() )
        {
            s.d = new Data( *d );
        }

        return s;
    }

}

