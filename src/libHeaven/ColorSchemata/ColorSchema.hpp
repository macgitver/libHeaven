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

#ifndef HEAVEN_COLOR_SCHEMATA_COLOR_SCHEMA_HPP
#define HEAVEN_COLOR_SCHEMATA_COLOR_SCHEMA_HPP

#include <QPalette>

#include "libHeaven/HeavenApi.h"
#include "libHeaven/ColorSchemata/ColorSet.hpp"
#include "libHeaven/ColorSchemata/ColorManager.hpp"

namespace Heaven
{

    class ColorSchemaPrivate;

    class HEAVEN_API ColorSchema : public QObject
    {
        Q_OBJECT
    public:
        ColorSchema();
        ~ColorSchema();

    signals:
        void modified();

    public:
        QColor get( ColorId id, QPalette::ColorGroup group = QPalette::Active ) const;
        QColor get( QPalette::ColorRole role, QPalette::ColorGroup group = QPalette::Active ) const;
        QColor get( const QByteArray& path, QPalette::ColorGroup group = QPalette::Active ) const;
        QColor get( const char* pszPath, QPalette::ColorGroup group = QPalette::Active ) const;

        void set( ColorId id, const QColor& color,
                  QPalette::ColorGroup group = QPalette::Active );

        void set( const QByteArray& path, const QColor& color,
                  QPalette::ColorGroup group = QPalette::Active );

        void set( const char* pszPath, const QColor& color,
                  QPalette::ColorGroup group = QPalette::Active );

        ColorId colorId( const QByteArray& path ) const;
        ColorId colorId( const char* pszPath ) const;

    public:
        void loadFile( QIODevice* iodevice );
        void loadString( const QString& data );
        QString saveString();

    private:
        ColorSchemaPrivate* d;
    };

    inline QColor ColorSchema::get( const QByteArray& path, QPalette::ColorGroup group ) const
    {
        return get( colorId( path ), group );
    }

    inline QColor ColorSchema::get( const char* pszPath, QPalette::ColorGroup group ) const
    {
        return get( colorId( pszPath ), group );
    }

    inline void ColorSchema::set( const QByteArray& path, const QColor& color,
                                  QPalette::ColorGroup group )
    {
        set( colorId( path ), color, group );
    }

    inline void ColorSchema::set( const char* pszPath, const QColor& color,
                                  QPalette::ColorGroup group )
    {
        set( colorId( pszPath ), color, group );
    }

    inline ColorId ColorSchema::colorId( const QByteArray& path ) const
    {
        return ColorManager::self().colorId( path );
    }

    inline ColorId ColorSchema::colorId(const char* pszPath) const
    {
        return ColorManager::self().colorId( pszPath );
    }

}

#endif
