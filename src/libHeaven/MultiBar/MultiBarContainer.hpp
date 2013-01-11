/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef HEAVEN_MULTI_BAR_CONTAINER_HPP
#define HEAVEN_MULTI_BAR_CONTAINER_HPP

#include <QWidget>

namespace Heaven
{

    class ViewContainerContent;
    class MultiBarContainerPrivate;
    class ToolBar;

    class MultiBarContainer : public QWidget
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
        MultiBarContainer();
        ~MultiBarContainer();

    public:
        BarPos barPos() const;
        void setBarPos( BarPos position );

    public:
        int addView( ViewContainerContent* view );
        int insertView( int index, ViewContainerContent* view );
        ViewContainerContent* takeView( int index );

    private slots:
        void viewChanged( int index );
        void viewToolBarChanged( Heaven::ToolBar* toolBar );
        void onCloseActiveView();

    private:
        MultiBarContainerPrivate* d;
    };

}

#endif
