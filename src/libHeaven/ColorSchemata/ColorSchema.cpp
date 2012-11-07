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

#include <QFile>
#include <QDomDocument>
#include <QStringBuilder>

#include "libHeaven/ColorSchemata/ColorSchema.hpp"
#include "libHeaven/ColorSchemata/ColorSchemaPrivate.hpp"
#include "libHeaven/ColorSchemata/ColorManagerPrivate.hpp"

namespace Heaven
{

    ColorSchemaPrivate::ColorSchemaPrivate()
    {
    }

    bool ColorSchemaPrivate::load( const QDomElement& el, const QByteArray& prefix )
    {
        QByteArray me = el.attribute( QLatin1String( "Id" ) ).toLatin1();

        if( el.tagName() == QLatin1String( "Group" ) )
        {
            ColorManagerPrivate::ensureGroupExists(
                prefix.isEmpty() ? me : prefix % '/' % me,
                el.attribute( QLatin1String( "Name" ) ),
                el.attribute( QLatin1String( "Order" ), QLatin1String( "-1" ) ).toInt() );

            QDomElement curEl = el.firstChildElement();
            while( curEl.isElement() )
            {
                if( !load( curEl, prefix.isEmpty() ? me : prefix % '/' % me ) )
                {
                    return false;
                }

                curEl = curEl.nextSiblingElement();
            }
        }
        else
        {
            ColorId id = ColorManager::self().colorId( prefix % '/' % me );
            if( id == -1 )
            {
                id = ColorManager::self().addColor( prefix, me,
                    el.attribute( QLatin1String( "Display" ) ) );
            }

            mColors.insert( QPair< ColorId, QPalette::ColorGroup >( id, QPalette::Active ),
                            QColor( el.attribute( QLatin1String( "Active" ) ) ) );

            mColors.insert( QPair< ColorId, QPalette::ColorGroup >( id, QPalette::Inactive ),
                            QColor( el.attribute( QLatin1String( "Inactive" ) ) ) );

            mColors.insert( QPair< ColorId, QPalette::ColorGroup >( id, QPalette::Disabled ),
                            QColor( el.attribute( QLatin1String( "Disabled" ) ) ) );
        }

        return true;
    }

    bool ColorSchemaPrivate::load( const QDomDocument& doc )
    {
        QDomElement docEl = doc.documentElement();
        QDomElement curEl = docEl.firstChildElement();
        while( curEl.isElement() )
        {
            if( !load( curEl, QByteArray() ) )
            {
                return false;
            }

            curEl = curEl.nextSiblingElement();
        }

        return true;
    }

    ColorSchema::ColorSchema()
    {
        d = new ColorSchemaPrivate;
        ColorManagerPrivate::sSelf->d->syncFromCorePalette( this );
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

        return d->mColors.value( QPair< ColorId, QPalette::ColorGroup >( id, group ), QColor() );
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

    bool ColorSchema::loadFile( const QString& name )
    {
        QFile f( name );
        if( !f.open( QFile::ReadOnly ) )
        {
            return false;
        }

        return loadFile( &f );
    }

    bool ColorSchema::loadFile( QIODevice* iodevice )
    {
        QDomDocument doc;

        if( !doc.setContent( iodevice ) )
        {
            return false;
        }

        return d->load( doc );
    }

    bool ColorSchema::loadString( const QString& data )
    {
        QDomDocument doc;
        doc.setContent( data );
        return d->load( doc );
    }

    QString ColorSchema::saveString()
    {
        QDomDocument doc( QLatin1String( "hcs" ) );

        return doc.toString();
    }

}
