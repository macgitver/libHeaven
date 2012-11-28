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

#ifndef HAVEN_ICON_HPP
#define HAVEN_ICON_HPP

#include <QPixmap>

#include "libHeaven/Icons/IconRef.hpp"

namespace Heaven
{

    class IconData;

    class HEAVEN_API Icon
    {
    public:
        Icon();
        Icon( const Icon& other );
        ~Icon();

        Icon& operator=( const Icon& other );

        bool operator==( const Icon& other ) const;
        bool operator!=( const Icon& other ) const;

    private:
        // no QExplicitlySharedDataPointer here, we want to put it to cache
        // whenever QESDP would put it to rest...
        void ref() const;
        void deref() const;
        IconData* d;
    };

}

#endif
