/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#include "Views/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    ViewWidget::ViewWidget( QWidget* parent )
        : QWidget( parent )
    {
    }

    bool ViewWidget::isContainerWidget() const
    {
        return false;
    }

    ContainerWidget::ContainerWidget( QWidget* parent )
        : ViewWidget( parent )
    {
    }

    void ContainerWidget::add( ViewWidget* widget )
    {
        insert( count(), widget );
    }

    bool ContainerWidget::isContainerWidget() const
    {
        return true;
    }

}
