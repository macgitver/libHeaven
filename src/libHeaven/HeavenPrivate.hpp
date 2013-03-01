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

#ifndef MGV_HEAVEN_HEAVEN_PRIVATE_H
#define MGV_HEAVEN_HEAVEN_PRIVATE_H

namespace Heaven
{

    enum UiObjectTypes
    {
        ActionType,
        ContainerType,
        MergePlaceType,
        MenuType,
        MenuBarType,
        ToolBarType,
        WidgetActionType,
        DynamicActionMergerType,

        SeparatorType
    };


    /**
     * @internal
     * @brief Grace-Period for ViewContext objects that are neither attached nor have an owner
     *
     * In seconds.
     */
    const int gGracePeriodContextShutdownOwnerless = 10;

    /**
     * @internal
     * @brief Grace-Period for ViewContext objects that are not attached but have an owner
     *
     * In seconds.
     */
    const int gGracePeriodContextShutdownUnattached = 30;

    /**
     * @internal
     * @brief Grace-Period for ViewContext objects that volunteered to be killed
     *
     * In seconds.
     */
    const int gGracePeriodContextShutdownVolunteered = 90;


    const bool gDebugContexts = true;

}

#define UUIDSTR_PRIMARY_WINDOW      "97302003-c519-43b6-aea1-26db56e99941"
#define UUIDSTR_PRIMARY_SPLITTER    "758c6df6-8153-4382-aae7-0f5603302b05"

#endif
