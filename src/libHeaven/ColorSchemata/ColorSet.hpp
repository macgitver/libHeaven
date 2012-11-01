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

#ifndef HEAVEN_COLOR_SCHEMATA_COLOR_SET_HPP
#define HEAVEN_COLOR_SCHEMATA_COLOR_SET_HPP

#include <QString>
#include <QHash>
#include <QList>

#include "libHeaven/HeavenApi.h"
#include "libHeaven/ColorSchemata/ColorManager.hpp"

namespace Heaven
{

    class HEAVEN_API ColorDef
    {
    public:
        ColorDef();
        ColorDef( ColorId id, const QByteArray& name, const QString& translatedName,
                  int sortOrder );
        ColorDef( const ColorDef& other );

    public:
        ColorId id() const;
        int sortOrder() const;
        QByteArray name() const;
        QString translatedName() const;

    private:
        ColorId mId;
        int mSortOrder;
        QByteArray mName;
        QString mTranslatedName;
    };

    class HEAVEN_API ColorSet
    {
    private:
        ColorSet( ColorSet* parent );
    protected:
        ColorSet();
        ~ColorSet();

    public:
        QByteArray path() const;
        QByteArray name() const;
        QString translatedName() const;
        int sortOrder() const;

        ColorSet* child( const QByteArray& name ) const;
        QList< ColorSet* > children() const;

        QHash< QByteArray, ColorDef > colorDefs() const;

        ColorSet* addSet( const QByteArray& name, const QString& translatedName,
                          int sortOrder = -1 );

        bool addColor( ColorId id, const QByteArray& name, const QString& translatedName,
                       int sortOrder = -1 );

        ColorId findId( const QList< QByteArray >& paths ) const;

    private:
        ColorSet* mParent;
        int mSortOrder;
        QHash< QByteArray, ColorSet* > mChildren;
        QByteArray mName;
        QString mTranslatedName;
        QHash< QByteArray, ColorDef > mColorIds;
    };

}

#endif
