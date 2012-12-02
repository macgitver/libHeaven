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

#ifndef HIC_PROPERTY_H
#define HIC_PROPERTY_H

#include <QVariant>
#include <QFlags>

enum HICPropertyType
{
    HICP_Any        = -1,
    HICP_NULL       = 0,
    HICP_String     = 1 << 0,
    HICP_TRString   = 1 << 1,
    HICP_Boolean    = 1 << 2,
    HICP_Integer    = 1 << 3
};

typedef QFlags< HICPropertyType > HICPropertyTypes;

class HICObject;

class HICProperty
{
public:
    HICProperty( const QVariant& v, HICPropertyType type );
    HICProperty();

public:
    QVariant value() const;
    HICPropertyType type() const;

public:
    static bool isPropertyAllowed( HICObject* object, const QString& name, HICPropertyType type );

private:
    QVariant mValue;
    HICPropertyType mType;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( HICPropertyTypes )

#endif
