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

#ifndef MGV_HEAVEN_VIEW_CONTAINER_CONTENT_H
#define MGV_HEAVEN_VIEW_CONTAINER_CONTENT_H

#include "libHeaven/HeavenApi.hpp"

class QWidget;

namespace Heaven
{

    class WindowStateBase;
    class ViewContainer;
    class View;

    /**
     * @class   ViewContainerContent
     * @brief   Interface / Base-Implementation for anything that can be contained
     *
     * This is a common secondary base class for View (inheriting QWidget) and ViewContainer
     * (inheriting QObject).
     *
     * It builds up an alternative hierarchy of the QWidget hierarchy; inserting some container
     * elements. This _should_ be simplified!
     */
    class HEAVEN_API ViewContainerContent
    {
    public:
        ViewContainerContent( const QString& identifier, ViewContainer* parent = NULL );
        virtual ~ViewContainerContent();

    public:
        virtual bool isContainer() const = 0;
        virtual View* asView();
        virtual ViewContainer* asContainer();
        virtual QWidget* widget() = 0;

        void setContainer( ViewContainer* parent );
        ViewContainer* container() const;

        QString identifier() const;

        QString handle() const;

    public:
        void setWindowState( WindowStateBase* wsBase );
        WindowStateBase* windowState();

    private:
        const QString       mIdentifier;
        ViewContainer*      mParentContainer;
        WindowStateBase*    mWindowState;
    };

}

#endif
