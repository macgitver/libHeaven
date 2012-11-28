/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include "libHeaven/Icons/Icon.hpp"
#include "libHeaven/Icons/IconPrivate.hpp"

namespace Heaven
{

    Icon::Icon()
    {
    }

    Icon::~Icon()
    {
        deref();
    }

    Icon::Icon( const Icon& other )
        : d( other.d )
    {
        ref();
    }

    Icon& Icon::operator=( const Icon& other )
    {
        if( d != other.d )
        {
            other.ref();
            deref();
            d = other.d;
        }
        return *this;
    }

    bool Icon::operator==( const Icon& other ) const
    {
        return d == other.d;
    }

    bool Icon::operator!=( const Icon& other ) const
    {
        return d != other.d;
    }

    void Icon::ref() const
    {
        if( d ) d->ref.ref();
    }

    void Icon::deref() const
    {
        if( d )
        {
            if( !d->ref.deref() )
            {
                // enslave our data to the cache.
                delete d; // for now
            }
        }
    }

}
