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

    #if defined(QT_DEBUG) && 0
    const bool gDebugContexts = true;
    const bool gDebugContextsVerbose = true;
    #else
    const bool gDebugContexts = false;
    const bool gDebugContextsVerbose = false;
    #endif

}

#endif
