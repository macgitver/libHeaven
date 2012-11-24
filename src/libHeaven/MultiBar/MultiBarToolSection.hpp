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

#ifndef HEAVEN_MULTI_BAR_TOOL_SECTION_HPP
#define HEAVEN_MULTI_BAR_TOOL_SECTION_HPP

#include "libHeaven/MultiBar/MultiBarSection.hpp"

namespace Heaven
{

    class ToolBar;
    class MultiBarSectionPrivate;

    class MultiBarToolSection : public MultiBarSection
    {
        Q_OBJECT
    public:
        MultiBarToolSection( QWidget* parent = 0 );

    public:
        void setToolBar( ToolBar* tb );
        ToolBar* toolBar() const;
    };

}

#endif
