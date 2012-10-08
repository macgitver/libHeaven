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

#include "libHeaven/ColorSchemata/ColorManager.hpp"
#include "libHeaven/ColorSchemata/ColorManagerPrivate.hpp"
#include "libHeaven/ColorSchemata/ColorSchemaEditor.hpp"
#include "ColorSchema.hpp"

namespace Heaven
{

    ColorManagerPrivate::ColorManagerPrivate()
    {
        mActiveSchema = NULL;
    }

    ColorManager::ColorManager()
    {
        d = new ColorManagerPrivate;
    }

    ColorManager::~ColorManager()
    {
        delete d;
    }

    ColorManager* ColorManagerPrivate::sSelf = NULL;

    ColorManager& ColorManager::self()
    {
        if( !ColorManagerPrivate::sSelf )
        {
            ColorManagerPrivate::sSelf = new ColorManager;
        }

        return *ColorManagerPrivate::sSelf;
    }

    ColorSchemaEditor* ColorManager::createEditorWidget()
    {
        return new ColorSchemaEditor;
    }
    
    ColorSchema* ColorManager::activeSchema()
    {
        return d->mActiveSchema;
    }

    QColor ColorManager::get( ColorId id, QPalette::ColorGroup group )
    {
        return self().activeSchema()->get( id, group );
    }

    QColor ColorManager::get( const QByteArray& path, QPalette::ColorGroup group )
    {
        ColorManager& cm( self() );
        return cm.activeSchema()->get( cm.colorId( path ), group );
    }

    QColor ColorManager::get( const char* pszPath, QPalette::ColorGroup group )
    {
        ColorManager& cm( self() );
        return cm.activeSchema()->get( cm.colorId( pszPath ), group );
    }

    ColorId ColorManager::colorId( const QByteArray& path ) const
    {
    }

    ColorId ColorManager::colorId( const char* pszPath ) const
    {
        return colorId( QByteArray( pszPath ) );
    }

    void ColorManager::addColorSet( const QByteArray& path, const QByteArray& name,
                                    const QString& translatedName )
    {
        
    }

    ColorId ColorManager::addColor( const QByteArray& path, const QString& colorName,
                                    const QString& translatedName )
    {
    }

}
