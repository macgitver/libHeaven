/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_HEAVEN_HEAVEN_H
#define MGV_HEAVEN_HEAVEN_H

#include "libHeaven/HeavenApi.h"

namespace Heaven
{

    enum Positions
    {
        Left,
        Right,
        Top,
        Bottom,
        Central
    };

    enum UiObjectTypes
    {
        ActionType,
        ContainerType,
        MergePlaceType,
        MenuType,
        MenuBarType,
        ToolBarType,
        WidgetActionType,

        SeparatorType
    };

    /**
     * @brief   These types classify a Heaven::View
     *
     *          The basic view type is Single. It's a view that can be opened and closed. When it's
     *          closed, it will forget about it's data. A global view is an addition to that. When
     *          it gets closed, it will remember it's data for a period of time. A type of Multiple
     *          means that more than one view will reference the same data.
     *
     *          Contrary to all of that, a Dependant view will provide additional value to the user
     *          which makes only sense in a specific context of another view.
     */
    enum ViewTypes
    {
        SingleViewType,
        GlobalViewType,
        MultipleViewType,
        DependantViewType
    };

}

#endif
