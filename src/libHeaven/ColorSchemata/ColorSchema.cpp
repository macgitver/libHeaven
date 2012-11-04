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

#include <QIODevice>
#include <QDomDocument>

#include "libHeaven/ColorSchemata/ColorSchema.hpp"
#include "libHeaven/ColorSchemata/ColorSchemaPrivate.hpp"
#include "libHeaven/ColorSchemata/ColorManagerPrivate.hpp"

namespace Heaven
{

    ColorSchemaPrivate::ColorSchemaPrivate()
    {
    }

    void ColorSchemaPrivate::load( const QDomDocument& doc )
    {
    }

    ColorSchema::ColorSchema()
    {
        d = new ColorSchemaPrivate;
    }

    ColorSchema::~ColorSchema()
    {
        delete d;
    }

    QColor ColorSchema::get( ColorId id, QPalette::ColorGroup group ) const
    {
        if( group != QPalette::Active &&
            group != QPalette::Disabled &&
            group != QPalette::Inactive )
        {
            return QColor();
        }


        return QColor();
    }

    void ColorSchema::set( ColorId id, const QColor& color, QPalette::ColorGroup group )
    {
        if( group != QPalette::Active &&
            group != QPalette::Disabled &&
            group != QPalette::Inactive )
        {
            return;
        }

        if( this == ColorManager::self().activeSchema() )
        {
            ColorManagerPrivate::syncToCorePalette();
        }
    }

    QColor ColorSchema::get( QPalette::ColorRole role, QPalette::ColorGroup group ) const
    {
        return get( ColorManager::role2Id( role ), group );
    }

    void ColorSchema::loadFile( QIODevice* iodevice )
    {
        QDomDocument doc;
        doc.setContent( iodevice );
        d->load( doc );
    }

    void ColorSchema::loadString( const QString& data )
    {
        QDomDocument doc;
        doc.setContent( data );
        d->load( doc );
    }

    QString ColorSchema::saveString()
    {
        QDomDocument doc( QLatin1String( "hcs" ) );

        return doc.toString();
    }

}
