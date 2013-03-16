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

#ifndef HEAVEN_ABSTRCAT_VIEW_WIDGET_HPP
#define HEAVEN_ABSTRCAT_VIEW_WIDGET_HPP

#include <QWidget>

#include "libHeaven/HeavenApi.hpp"
#include "libHeaven/Heaven.hpp"

#include "libHeaven/CentralUI/Views/ViewIdentifier.hpp"

namespace Heaven
{

    class ContainerWidget;

    /**
     * @class   AbstractViewWidget
     * @ingroup CentralUI
     * @brief   Base class for all Central UI Widgets
     *
     *
     */
    class HEAVEN_API AbstractViewWidget : public QWidget
    {
        Q_OBJECT
    public:
        AbstractViewWidget( const ViewIdentifier& identifier );

    public:
        virtual bool isContainerWidget() const;

    public:
        ViewIdentifier identifier() const;
        ContainerWidget* parentContainer();
        void setParentContainer( ContainerWidget* container );

    public:
        ContainerWidget* asContainerWidget();

    private:
        const ViewIdentifier    mIdentifier;        //!< Identifier of this View / ContainerWidget
        ContainerWidget*        mParentContainer;   //!< The parent container
    };

}

#endif
