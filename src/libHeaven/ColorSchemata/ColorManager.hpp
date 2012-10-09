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

    typedef qint32 ColorId;

    class ColorSchema;
    class ColorManagerPrivate;

    class HEAVEN_API ColorManager : public QObject
    {
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
        static QColor get( ColorId id,
                           QPalette::ColorGroup group = QPalette::Active );

        static QColor get( const QByteArray& path,
                           QPalette::ColorGroup group = QPalette::Active );

        static QColor get( const char* pszPath,
                           QPalette::ColorGroup group = QPalette::Active );

    public:
        ColorId colorId( const QByteArray& path ) const;
        ColorId colorId( const char* pszPath ) const;

        bool addColorSet( const QByteArray& path, const QByteArray& name,
                          const QString& translatedName, int sortOrder = -1 );

        ColorId addColor( const QByteArray& path, const QByteArray& colorName,
                          const QString& translatedName, int sortOrder = -1 );

        QList< QByteArray > sortedColors( const QByteArray& path ) const;
        QList< QByteArray > sortedChildren( const QByteArray& path ) const;

    private:
        ColorManager();
        ColorManagerPrivate* d;
    };

}

#endif

