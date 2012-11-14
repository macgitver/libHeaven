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

#ifndef HEAVEN_COLOR_SCHEMATA_COLOR_MANAGER_HPP
#define HEAVEN_COLOR_SCHEMATA_COLOR_MANAGER_HPP

#include <QObject>
#include <QStringList>
#include <QPalette>

class QByteArray;

#include "libHeaven/HeavenApi.h"

namespace Heaven
{

    typedef qint16 ColorId;

    class ColorSchema;
    class ColorSchemaEditor;
    class ColorManagerPrivate;

    class HEAVEN_API ColorManager : public QObject
    {
        friend class ColorSchema;
        friend class ColorSchemaEditor;
        friend class ColorManagerPrivate;
        Q_OBJECT
    public:
        static ColorManager& self();
        ~ColorManager();

    public:
        void initialize();
        QWidget* createEditorWidget();

    public:
        ColorSchema* activeSchema();

    public:
        void addSchemaFromFile( const QString& name, const QString& fileName );
        void setActiveSchema( const QString& name );
        QStringList schemata() const;

    public:
        static ColorId role2Id( QPalette::ColorRole role );

        static QColor get( ColorId id,
                           QPalette::ColorGroup group = QPalette::Active );

        static QColor get( QPalette::ColorRole role,
                           QPalette::ColorGroup group = QPalette::Active );

        static QColor get( const QByteArray& path,
                           QPalette::ColorGroup group = QPalette::Active );

        static QColor get( const char* pszPath,
                           QPalette::ColorGroup group = QPalette::Active );

    public:
        ColorId colorId( const QByteArray& path ) const;
        ColorId colorId( const char* pszPath ) const;

        void addColorSet( const QByteArray& path, const QByteArray& name,
                          const QString& translatedName, int sortOrder = -1 );

        ColorId addColor( const QByteArray& path, const QByteArray& colorName,
                          const QString& translatedName, int sortOrder = -1 );

        QList< QByteArray > sortedColors( const QByteArray& path ) const;
        QList< QByteArray > sortedChildren( const QByteArray& path ) const;

        QString translatedPathName( const QByteArray& path ) const;
        QString translatedColorName( const QByteArray& path,
                                     const QByteArray& color ) const;

    protected:
        bool eventFilter( QObject* o, QEvent* e );

    private:
        ColorManager();
        ColorManagerPrivate* d;
    };

}

#endif

