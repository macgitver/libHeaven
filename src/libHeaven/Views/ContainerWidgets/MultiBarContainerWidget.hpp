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

#include "libHeaven/Heaven.hpp"
#include "libHeaven/Views/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    class ViewContainerContent;
    class MultiBarContainerWidgetPrivate;
    class ToolBar;

    class MultiBarContainerWidget : public ContainerWidget
    {
        Q_OBJECT
    public:
        MultiBarContainerWidget();
        ~MultiBarContainerWidget();

    public:
        Positions barPosition() const;
        void setBarPosition( Positions position );

    public:
        int insert( int index, AbstractViewWidget* view );
        AbstractViewWidget* takeAt( int index );
        int indexOf( AbstractViewWidget* widget ) const;
        int count() const;

        ContainerTypes containerType() const;

    private slots:
        void viewChanged( int index );
        void viewToolBarChanged( Heaven::ToolBar* toolBar );
        void onCloseActiveView();

    private:
        MultiBarContainerWidgetPrivate* d;
    };

}

#endif
