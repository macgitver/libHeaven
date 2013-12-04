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

#ifndef HEAVEN_ADVANCED_CACHE_HPP
#define HEAVEN_ADVANCED_CACHE_HPP

#include <QMap>
#include <QHash>

namespace Heaven
{

    // Totally unused and untested class!

    template< class Key, class T >
    class AdvancedCache
    {
    public:
        AdvancedCache( int maxCost = 100 );
        ~AdvancedCache();

    public:
        void clear();

        void insert( const Key& key, T* data, int holdCost = 1, int anewCost = 1 );
        void remove( const Key& key );
        T* take( const Key& key );
        bool contains( const Key& key ) const;
        bool isEmpty() const;

        int totalCost() const;
        int maxCost() const;

        void setMaxCost( int maxCost );

    private:
        void bringBalanceToTheForce();

    private:
        struct Data
        {
            T*      payload;
            int     holdCost;
            int     anewCost;
        };
        QHash< Key, Data >  content;
        QMap< int, int >    tops;       //!< Maps "anew"-cost to # of entries
        int                 total;      //!< Current total "hold" cost"
        int                 max;        //!< Maximum "hold" cost
    };

    template< class Key, class T >
    inline AdvancedCache< Key, T >::AdvancedCache( int maxCost )
    {
        total = 0;
        max = maxCost;
    }

    template< class Key, class T >
    inline AdvancedCache< Key, T >::~AdvancedCache()
    {
        clear();
    }

    template< class Key, class T >
    inline int AdvancedCache< Key, T >::totalCost() const
    {
        return total;
    }

    template< class Key, class T >
    inline int AdvancedCache< Key, T >::maxCost() const
    {
        return max;
    }

    template< class Key, class T >
    inline void AdvancedCache< Key, T >::setMaxCost( int maxCost )
    {
        max = maxCost;
        while( max > total && content.count() )
        {
            bringBalanceToTheForce();
        }
    }

    template< class Key, class T >
    inline bool AdvancedCache< Key, T >::isEmpty() const
    {
        return content.count() == 0;
    }

    template< class Key, class T >
    inline void AdvancedCache< Key, T >::clear()
    {
        foreach( const Data& data, content )
        {
            delete data.payload;
        }
        total = 0;

        tops.clear();
        content.clear();
    }

    template< class Key, class T >
    inline void AdvancedCache< Key, T >::insert( const Key& key, T* payload, int holdCost,
                                                 int anewCost )
    {
        // Optimize this. If we have a key already, we must do:
        //      while( total + newHoldCost - oldHoldCost > max )
        // but that might remove our key.
        while( total + holdCost > max && content.count() )
        {
            bringBalanceToTheForce();
        }

        if( content.contains( key ) )
        {
            Data& data = content[ key ];

            if( data.payload != payload )
            {
                delete data.payload;
                data.payload = payload;
            }

            total -= data.holdCost;

            if( data.anewCost != anewCost )
            {
                if( tops[ data.anewCost ]-- == 1 )
                {
                    tops.remove( data.anewCost );
                }

                tops[ anewCost ]++;
            }

            data.holdCost = holdCost;
            data.anewCost = anewCost;
        }
        else
        {
            Data data;
            data.payload = payload;
            data.holdCost = holdCost;
            data.anewCost = anewCost;

            tops[ anewCost ]++;
        }

        total += holdCost;
    }

    template< class Key, class T >
    inline void AdvancedCache< Key, T >::remove( const Key& key )
    {
        delete take( key );
    }

    template< class Key, class T >
    inline T* AdvancedCache< Key, T >::take( const Key& key )
    {
        if( !content.contains( key ) )
        {
            return NULL;
        }

        Data data = content.value( key );
        total -= content.holdCost;
        content.remove( key );

        if( tops[ data.anewCost ]-- == 1 )
        {
            tops.remove( data.anewCost );
        }

        return data.payload;
    }

    template< class Key, class T >
    inline bool AdvancedCache< Key, T >::contains( const Key& key ) const
    {
        return content.contains( key );
    }

    template< class Key, class T >
    inline void AdvancedCache< Key, T >::bringBalanceToTheForce()
    {
        if( content.count() == 0 )
        {
            return;
        }

        QMapIterator< int, int > l( tops ); l.toBack();
        int atop = l.key();

        typename QHash< Key, Data >::Iterator it = content.first();
        while( it != content.end() )
        {
            if( it.value().anewCost == atop )
            {
                remove( it.key() );
                break;
            }
        }
    }

}

#endif
