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

#ifndef HEAVEN_COLOR_SCHEMATA_COLOR_MANAGER_PRIVATE_HPP
#define HEAVEN_COLOR_SCHEMATA_COLOR_MANAGER_PRIVATE_HPP

#include <QStringList>

#include "libHeaven/ColorSchemata/ColorSet.hpp"

namespace Heaven
{

    class ColorManager;
    class ColorSchema;

    class RootColorSet : public ColorSet
    {
    public:
        RootColorSet()
            : ColorSet()
        {}

        ~RootColorSet()
        {}
    };

    class ColorManagerPrivate
    {
    public:
        ColorManagerPrivate();

    public:
        RootColorSet        mRootSet;
        ColorSchema*        mActiveSchema;
        QStringList         mKnownSchemata;

        static ColorManager* sSelf;
    };

}

#endif
