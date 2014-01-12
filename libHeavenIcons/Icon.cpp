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

#include "libHeavenIcons/Icon.hpp"
#include "libHeavenIcons/IconPrivate.hpp"

namespace Heaven
{

    Icon::Icon()
    {
    }

    Icon::Icon( const IconRef& ref, const QPixmap& pixmap )
    {
        d = new Data;
        d->iconRef = ref;
        d->icon = pixmap;
    }

    Icon::~Icon()
    {
    }

    Icon::Icon( const Icon& other )
        : d( other.d )
    {
    }

    Icon& Icon::operator=( const Icon& other )
    {
        d = other.d;
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

    bool Icon::isValid() const
    {
        return d.data() != NULL;
    }

    QPixmap Icon::pixmap() const
    {
        return d ? d->icon : QPixmap();
    }

    IconRef Icon::iconRef() const
    {
        return d ? d->iconRef : IconRef();
    }

}
