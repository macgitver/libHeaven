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

#ifndef HAVEN_ICON_MANAGER_HPP
#define HAVEN_ICON_MANAGER_HPP

#include "libHeavenIcons/libHeavenIconsAPI.hpp"

namespace Heaven
{

    class Icon;
    class IconRef;
    class IconDefaultProvider;
    class IconProvider;
    class IconManagerPrivate;

    class HEAVEN_ICONS_API IconManager
    {
    private:
        IconManager();

    public:
        static IconManager& self();
        ~IconManager();

    public:
        IconDefaultProvider* defaultProvider() const;

        IconProvider* provider( const QString& name ) const;
        void registerProvider( IconProvider* provider );
        void unregisterProvider( IconProvider* provider );

        Icon icon( const IconRef& ref );

    private:
        static IconManager* sSelf;
        IconManagerPrivate* d;
    };

}

#endif
