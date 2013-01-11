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

#ifndef HIC_OBJECT_H
#define HIC_OBJECT_H

#include <QStringList>

#include "HICProperty.h"

class HICObject;

class HICObjects : public QList< HICObject* >
{
public:
    HICObjects();

public:
    HICObjects byType( ObjectTypes type ) const;
};

class HICObject
{
public:
    HICObject( ObjectTypes type );

public:
    ObjectTypes type();

    void setName( const QString& name );

    QString name() const;

    bool hasProperty( const QString& name, HICPropertyType type = HICP_Any ) const;

    HICProperty getProperty( const QString& name ) const;

    QStringList propertyNames() const;

    void addProperty( QString name, HICProperty p );
    void addContent( HICObject* content );
    HICObjects content() const;
    HICObjects content( ObjectTypes type ) const;
    bool hasReferenceTo( HICObject* obj ) const;

private:
    ObjectTypes mType;
    QString mName;
    QHash< QString, HICProperty > mProperties;
    HICObjects mContent;
};

typedef QHash< QString, HICObject* > HIDObjectHash;

class HIDModel
{
public:
    HIDModel();

public:
    HIDObjectHash& objects();
    const HIDObjectHash& objects() const;

    HICObjects allObjects( ObjectTypes byType ) const;

private:
    HIDObjectHash mObjects;
};

#endif
