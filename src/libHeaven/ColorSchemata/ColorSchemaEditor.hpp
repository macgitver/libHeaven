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

#ifndef HEAVEN_COLOR_SCHEMATA_COLOR_SCHEMA_EDITOR_HPP
#define HEAVEN_COLOR_SCHEMATA_COLOR_SCHEMA_EDITOR_HPP

#include "libHeaven/HeavenApi.h"

#include <QWidget>

namespace Ui
{
    class ColorSchemaEditor;
}

namespace Heaven
{

    class ColorSchemaEditor : public QWidget
    {
        Q_OBJECT
    public:
        ColorSchemaEditor( QWidget* parent = 0 );
        ~ColorSchemaEditor();

    private:
        Ui::ColorSchemaEditor* ui;
    };
}

#endif
