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

#include <QApplication>
#include <QStringBuilder>
#include <QMap>

#include "libHeavenColors/ColorManager.hpp"
#include "libHeavenColors/ColorSet.hpp"
#include "libHeavenColors/ColorManagerPrivate.hpp"
#include "libHeavenColors/ColorSchemaEditor.hpp"
#include "libHeavenColors/ColorSchema.hpp"

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
        ColorId id = reserveId();
        mRootSet.child( "General" )->addColor( id, name, translatedName );
        mStockMap.append( StockEntry( cr, id ) );
    }

    ColorId ColorManagerPrivate::reserveId()
    {
        return mNextId++;
    }

    void ColorManagerPrivate::ensureGroupExists( const QByteArray& path,
                                                 const QString& translatedName, int sortOrder )
    {
        QList< QByteArray > paths = path.split( '/' );
        ColorSet* set = &sSelf->d->mRootSet;
        for( int i = 0; i < paths.count(); i++ )
        {
            ColorSet* set2 = set->child( paths[ i ] );
            if( set2 )
            {
                set = set2;
            }
            else
            {
                if( i == paths.count() - 2 )
                    set = set->addSet( paths[ i ], translatedName, sortOrder );
                else
                    set = set->addSet( paths[ i ], QString::fromLatin1( paths[ i ].constData() ) );
                Q_ASSERT( set );
            }
        }

        sSelf->d->mRootSet.dump();
    }

    void ColorManagerPrivate::syncFromCorePalette( ColorSchema* schema )
    {
        QPalette p = QApplication::palette();
        foreach( StockEntry se, mStockMap )
        {
            schema->set( se.second, p.color( QPalette::Active, se.first ),
                         QPalette::Active );

            schema->set( se.second, p.color( QPalette::Inactive, se.first ),
                         QPalette::Inactive );

            schema->set( se.second, p.color( QPalette::Disabled, se.first ),
                         QPalette::Disabled );
        }
    }

    void ColorManagerPrivate::syncToCorePalette()
    {
        ColorSchema* as = sSelf->activeSchema();

        bool modified = false;
        QPalette p = QApplication::palette();

        foreach( StockEntry se, sSelf->d->mStockMap )
        {
            QColor newClr = as->get( se.second, QPalette::Active );
            if( p.color( QPalette::Active, se.first ) != newClr )
            {
                p.setColor( QPalette::Active, se.first, newClr );
                modified = true;
            }

            newClr = as->get( se.second, QPalette::Inactive );
            if( p.color( QPalette::Inactive, se.first ) != newClr )
            {
                p.setColor( QPalette::Inactive, se.first, newClr );
                modified = true;
            }

            newClr = as->get( se.second, QPalette::Disabled );
            if( p.color( QPalette::Disabled, se.first ) != newClr )
            {
                p.setColor( QPalette::Disabled, se.first, newClr );
                modified = true;
            }
        }

        if( modified )
        {
            QApplication::setPalette( p );
        }
    }

    QStringList ColorManagerPrivate::knownSchemata() const
    {
        return mKnownSchemata.keys();
    }

    ColorManager::ColorManager()
    {
        d = new ColorManagerPrivate;

        qApp->installEventFilter( this );
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

    ColorId ColorManager::role2Id( QPalette::ColorRole role )
    {
        Q_ASSERT( ColorManagerPrivate::sSelf );
        foreach( ColorManagerPrivate::StockEntry se, ColorManagerPrivate::sSelf->d->mStockMap )
        {
            if( se.first == role )
            {
                return se.second;
            }
        }

        return 0;
    }

    QColor ColorManager::get( ColorId id, QPalette::ColorGroup group )
    {
        return self().activeSchema()->get( id, group );
    }

    QColor ColorManager::get( QPalette::ColorRole role, QPalette::ColorGroup group )
    {
        return self().activeSchema()->get( role, group );
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

    void ColorManager::addColorSet( const QByteArray& path, const QByteArray& name,
                                    const QString& translatedName, int sortOrder )
    {
        ColorManagerPrivate::ensureGroupExists( path % '/' % name, translatedName, sortOrder );
    }

    ColorId ColorManager::addColor( const QByteArray& path, const QByteArray& colorName,
                                    const QString& translatedName, int sortOrder )
    {
        QList< QByteArray > paths = path.split( '/' );
        ColorSet* set = &d->mRootSet;
        for( int i = 0; i < paths.count(); i++ )
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
        QList< QByteArray > children;

        ColorSet* set = &d->mRootSet;
        if( !path.isEmpty() )
        {
            QList< QByteArray > paths = path.split( '/' );
            for( int i = 0; i < paths.count(); i++ )
            {
                set = set ? set->child( paths[ i ] ) : NULL;
            }
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

    QString ColorManager::translatedPathName( const QByteArray& path ) const
    {
        ColorSet* set = &d->mRootSet;

        if( !path.isEmpty() )
        {
            QList< QByteArray > paths = path.split( '/' );
            for( int i = 0; i < paths.count(); i++ )
            {
                set = set ? set->child( paths[ i ] ) : NULL;
            }
        }

        return set ? set->translatedName() : QString();
    }

    QString ColorManager::translatedColorName( const QByteArray& path,
                                               const QByteArray& color ) const
    {
        ColorSet* set = &d->mRootSet;

        if( !path.isEmpty() )
        {
            QList< QByteArray > paths = path.split( '/' );
            for( int i = 0; i < paths.count(); i++ )
            {
                set = set ? set->child( paths[ i ] ) : NULL;
            }
        }

        if( !set )
        {
            return QString();
        }

        return set->translatedColorName( color );
    }

    bool ColorManager::eventFilter( QObject* o, QEvent* e )
    {
        if( o == qApp && e->type() == QEvent::PaletteChange )
        {
            d->syncFromCorePalette( d->mActiveSchema );
        }

        return QObject::eventFilter( o, e );
    }

    void ColorManager::addSchemaFromFile( const QString& name, const QString& fileName )
    {
        if( d->mKnownSchemata.contains( name ) )
        {
            return;
        }

        ColorSchema* schema = new ColorSchema( name );
        schema->loadFile( fileName );
        d->mKnownSchemata.insert( name, schema );
    }

    void ColorManager::setActiveSchema( const QString& name )
    {
        ColorSchema* s = d->mKnownSchemata.value( name, NULL );
        if( !s )
        {
            return;
        }

        d->mActiveSchema = s;
    }

    QStringList ColorManager::schemata() const
    {
        return d->mKnownSchemata.keys();
    }

}
