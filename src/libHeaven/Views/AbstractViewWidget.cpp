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

#include "libHeaven/Views/AbstractViewWidget.hpp"
#include "libHeaven/Views/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    AbstractViewWidget::AbstractViewWidget( const QString& identifier )
        : QWidget( NULL )
        , mIdentifier( identifier )
    {
    }

    bool AbstractViewWidget::isContainerWidget() const
    {
        return false;
    }

    ContainerWidget* AbstractViewWidget::parentContainer()
    {
        return mParentContainer;
    }

    QString AbstractViewWidget::identifier() const
    {
        return mIdentifier;
    }

    void AbstractViewWidget::setParentContainer( ContainerWidget* container )
    {
        mParentContainer = container;
    }

    ContainerWidget* AbstractViewWidget::asContainerWidget()
    {
        return isContainerWidget() ? static_cast< ContainerWidget* >( this ) : NULL;
    }

}

