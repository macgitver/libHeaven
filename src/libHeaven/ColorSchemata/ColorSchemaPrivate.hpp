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

#include <QString>
#include <QHash>
#include <QPalette>

class QDomDocument;

#include "libHeaven/ColorSchemata/ColorSchema.hpp"
#include "libHeaven/ColorSchemata/ColorSet.hpp"

namespace Heaven
{

    class ColorSchemaPrivate
    {
    public:
        ColorSchemaPrivate();

    public:
        bool load( const QDomDocument& doc );

    private:
        bool load( const QDomElement& el, const QByteArray& prefix );

    public:
        QHash< QPair< ColorId, QPalette::ColorGroup >, QColor > mColors;
        QString mName;
    };

}
