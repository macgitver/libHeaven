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

#include <QMap>

#include "libHeaven/ColorSchemata/ColorManager.hpp"
#include "libHeaven/ColorSchemata/ColorSet.hpp"
#include "libHeaven/ColorSchemata/ColorManagerPrivate.hpp"
#include "libHeaven/ColorSchemata/ColorSchemaEditor.hpp"
#include "libHeaven/ColorSchemata/ColorSchema.hpp"

namespace Heaven
{

    ColorManagerPrivate::ColorManagerPrivate()
    {
        mActiveSchema = NULL;
        mNextId = 1;

        mRootSet.addSet( "General", ColorManager::trUtf8( "General" ) );

        importQtColorRole( QPalette::Window,
                           "Window",
                           ColorManager::trUtf8( "Window" ) );

        importQtColorRole( QPalette::WindowText,
                           "Window-Text",
                           ColorManager::trUtf8( "Window-Text" ) );
    }

    void ColorManagerPrivate::importQtColorRole( QPalette::ColorRole cr, const QByteArray& name,
                                                 const QString& translatedName )
    {
        mRootSet.child( "General" )->addColor( reserveId(), name, translatedName );
    }

    ColorId ColorManagerPrivate::reserveId()
    {
        return mNextId++;
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

    QWidget* ColorManager::createEditorWidget()
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
        return d->mRootSet.findId( path.split( '/' ) );
    }

    ColorId ColorManager::colorId( const char* pszPath ) const
    {
        return colorId( QByteArray( pszPath ) );
    }

    bool ColorManager::addColorSet( const QByteArray& path, const QByteArray& name,
                                    const QString& translatedName, int sortOrder )
    {
        QList< QByteArray > paths = path.split( '/' );
        ColorSet* set = &d->mRootSet;
        for( int i = 0; i < paths.count() - 1; i++ )
        {
            set = set->child( paths[ i ] );
            if( !set )
            {
                return false;
            }
        }

        set->addSet( name, translatedName, sortOrder );
        return true;
    }

    ColorId ColorManager::addColor( const QByteArray& path, const QByteArray& colorName,
                                    const QString& translatedName, int sortOrder )
    {
        QList< QByteArray > paths = path.split( '/' );
        ColorSet* set = &d->mRootSet;
        for( int i = 0; i < paths.count() - 1; i++ )
        {
            set = set->child( paths[ i ] );
            if( !set )
            {
                return -1;
            }
        }

        ColorId id = d->reserveId();
        set->addColor( id, colorName, translatedName, sortOrder );
        return id;
    }

    QList< QByteArray > ColorManager::sortedColors( const QByteArray& path ) const
    {
        QList< QByteArray > paths = path.split( '/' );
        QList< QByteArray > children;

        ColorSet* set = &d->mRootSet;
        for( int i = 0; i < paths.count(); i++ )
        {
            set = set ? set->child( paths[ i ] ) : NULL;
        }

        if( set )
        {
            QMap< int, QByteArray > sorter;
            foreach( ColorDef def, set->colorDefs() )
            {
                sorter.insert( def.sortOrder(), def.name() );
            }

            children = sorter.values();
        }

        return children;
    }

    QList< QByteArray > ColorManager::sortedChildren( const QByteArray& path ) const
    {
        QList< QByteArray > paths = path.split( '/' );
        QList< QByteArray > children;

        ColorSet* set = &d->mRootSet;
        for( int i = 0; i < paths.count(); i++ )
        {
            set = set ? set->child( paths[ i ] ) : NULL;
        }

        if( set )
        {
            QMap< int, QByteArray > sorter;
            foreach( ColorSet* set2, set->children() )
            {
                sorter.insert( set2->sortOrder(), set2->name() );
            }

            children = sorter.values();
        }

        return children;
    }

}
