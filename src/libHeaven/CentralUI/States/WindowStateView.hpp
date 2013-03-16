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

#ifndef HEAVEN_WINDOW_STATE_VIEW_HPP
#define HEAVEN_WINDOW_STATE_VIEW_HPP

#include "libHeaven/CentralUI/States/WindowState.hpp"

namespace Heaven
{

    class WindowStateView : public WindowState
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateView > Ptr;

    public:
        WindowStateView( WindowState* parent );
        WindowStateView( WindowState* parent, QDomElement& el );

    public:
        Type type() const;

        void updateConfig();

    protected:
        void save( QDomElement& elParent ) const;
    };

}

#endif
