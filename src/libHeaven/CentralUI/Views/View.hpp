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

#ifndef HEAVEN_VIEW_HPP
#define HEAVEN_VIEW_HPP

#include <QWidget>
#include <QPointer>

#include "libHeaven/Heaven.hpp"
#include "libHeaven/Actions/ToolBar.hpp"
#include "libHeaven/CentralUI/ContainerWidgets/ContainerWidget.hpp"

namespace Heaven
{

    class View;
    class ToolBar;
    class WindowStateView;
    class MultiBarContainerWidget;

    class HEAVEN_API View : public AbstractViewWidget
    {
        Q_OBJECT
        friend class MultiBarContainerWidget;
    public:
        View( const ViewIdentifier& identifier );
        ~View();

    public:
        QString viewName() const;
        void setViewName( const QString& name );

        void setToolBar( ToolBar* tb );
        ToolBar* toolBar() const;

        void setWidget( QWidget* widget );
        QWidget* widget();

    signals:
        void nameChanged( const QString& viewName );
        void toolBarChanged( Heaven::ToolBar* toolBar );

    public:
        virtual void closeView();

    protected:
        virtual void aboutToRemove();

    private:
        void queueRelayouting();
    private slots:
        void performQueuedRelayouting();

    private:
        QString             mViewName;
        QPointer< ToolBar > mToolBar;
        QPointer< QWidget > mWidget;
        bool                mRelayoutingIsQueued    : 1;
        bool                mRelayoutingForced      : 1;
        bool                mToolBarInOwnLayout     : 1;
    };

}

#endif
