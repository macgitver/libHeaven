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

#ifndef HEAVEN_VIEW_H
#define HEAVEN_VIEW_H

#include <QWidget>
#include <QPointer>

#include "libHeaven/Heaven.hpp"
#include "libHeaven/Actions/ToolBar.h"
#include "libHeaven/Views/ContainerWidgets/ContainerWidget.hpp"
#include "libHeaven/Views/ViewContainerContent.h"

namespace Heaven
{
    class ViewContainer;
    class View;
    class ToolBar;
    class WindowStateView;

    class HEAVEN_API View
            : public ViewWidget
            , public ViewContainerContent
    {
        Q_OBJECT
    public:
        View( const QString& identifier, ViewTypes type = SingleViewType );
        ~View();

    public:
        ViewTypes type() const;

    public:
        QString viewName() const;
        void setViewName( const QString& name );

        void setToolBar( ToolBar* tb );
        ToolBar* toolBar() const;

    signals:
        void nameChanged( const QString& viewName );
        void toolBarChanged( Heaven::ToolBar* toolBar );

    public:
        virtual void closeView();

    protected:
        virtual void aboutToRemove();

    public: // ContainerContent Interface
        bool isContainer() const;
        View* asView();
        QWidget* widget();

    private:
        ViewContainer*      mContainer;
        QString             mViewName;
        ViewTypes           mType;
        QPointer< ToolBar > mToolBar;
    };

}

#endif
