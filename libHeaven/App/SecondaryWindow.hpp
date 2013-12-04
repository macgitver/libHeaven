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

#ifndef MGV_HEAVEN_SECONDARY_WINDOW_HPP
#define MGV_HEAVEN_SECONDARY_WINDOW_HPP

#include <QMainWindow>

#include "libHeaven/App/HeavenWindow.hpp"

namespace Heaven
{

    class SecondaryWindowPrivate;

    /**
     * @class       SecondaryWindow
     * @brief       Window containing Heaven views, but no FooterWidget or Menu
     *
     * @todo    Implement layouting of SecondaryWindow.
     *
     */
    class HEAVEN_API SecondaryWindow : public HeavenWindow
    {
        friend class SecondaryWindowPrivate;
        Q_OBJECT
    public:
        SecondaryWindow( const ViewIdentifier& handle );

    public:
        bool isPrimary() const;
    };

    typedef QList< SecondaryWindow* > SecondaryWindows;

}

#endif
