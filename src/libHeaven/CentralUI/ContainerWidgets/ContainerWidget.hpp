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

#ifndef HEAVEN_CONTAINER_WIDGET_HPP
#define HEAVEN_CONTAINER_WIDGET_HPP

#include "libHeaven/CentralUI/Views/AbstractViewWidget.hpp"

namespace Heaven
{

    class HEAVEN_API ContainerWidget : public AbstractViewWidget
    {
        Q_OBJECT
    public:
        ContainerWidget( const ViewIdentifier& identifier );

    public:
        void add( AbstractViewWidget* widget );
        virtual int insert( int pos, AbstractViewWidget* widget ) = 0;
        virtual AbstractViewWidget* takeAt( int pos ) = 0;
        virtual int indexOf( AbstractViewWidget* widget ) const = 0;
        virtual int count() const = 0;
        virtual AbstractViewWidget* widget( int index ) = 0;
        void take( AbstractViewWidget* widget );

        bool isContainerWidget() const;

        virtual ContainerTypes containerType() const = 0;
    };

}

#endif
