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

#ifndef HEAVEN_MULTI_BAR_CONTAINER_WIDGET_HPP
#define HEAVEN_MULTI_BAR_CONTAINER_WIDGET_HPP

#include "Views/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    class ViewContainerContent;
    class MultiBarContainerWidgetPrivate;
    class ToolBar;

    class MultiBarContainerWidget : public ContainerWidget
    {
        Q_OBJECT
    public:
        enum BarPos
        {
            North,
            South,
            West,
            East
        };

    public:
        MultiBarContainerWidget();
        ~MultiBarContainerWidget();

    public:
        BarPos barPos() const;
        void setBarPos( BarPos position );

    public:
        int insert( int index, ViewWidget* view );
        ViewWidget* takeAt( int index );
        int count() const;

    private slots:
        void viewChanged( int index );
        void viewToolBarChanged( Heaven::ToolBar* toolBar );
        void onCloseActiveView();

    private:
        MultiBarContainerWidgetPrivate* d;
    };

}

#endif
