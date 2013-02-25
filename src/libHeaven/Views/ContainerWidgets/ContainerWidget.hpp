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

#include <QWidget>

#include "libHeaven/HeavenApi.hpp"
#include "libHeaven/Heaven.hpp"

namespace Heaven
{

    // TODO: Move this class to a public header
    class HEAVEN_API ViewWidget : public QWidget
    {
        Q_OBJECT
    public:
        ViewWidget( QWidget* parent );

    public:
        virtual bool isContainerWidget() const;
    };

    class ContainerWidget : public ViewWidget
    {
        Q_OBJECT
    public:
        ContainerWidget( QWidget* parent );

    public:
        void add( ViewWidget* widget );
        virtual int insert( int pos, ViewWidget* widget ) = 0;
        virtual ViewWidget* takeAt( int pos ) = 0;
        virtual int count() const = 0;

        bool isContainerWidget() const;

        virtual ContainerTypes containerType() const = 0;
    };

}

#endif
