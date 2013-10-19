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

#include "libHeaven/CentralUI/States/WindowStateRoot.hpp"

namespace Heaven
{

    WindowStateRoot::WindowStateRoot()
        : WindowState( NULL )
        , mMode(NULL)
        , mIsReadOnly(false)
    {
    }

    WindowStateRoot::WindowStateRoot( const QDomElement& elParent )
        : WindowState( NULL )
        , mMode(NULL)
        , mIsReadOnly(false)
    {
        load( elParent );
    }

    WindowStateRoot::WindowStateRoot(WindowStateRoot* cloneFrom)
        : WindowState(NULL, cloneFrom)
    {
        mMode = cloneFrom->mMode;
        mIsReadOnly = true;
    }

    WindowStateRoot* WindowStateRoot::clone(WindowState* toParent)
    {
        Q_ASSERT(toParent == NULL);
        return new WindowStateRoot(this);
    }

    WindowStateRoot::~WindowStateRoot()
    {
    }

    WindowState::Type WindowStateRoot::type() const
    {
        return WSRoot;
    }


    void WindowStateRoot::save( QDomElement& elParent ) const
    {
        for( int i = 0; i < childrenCount(); ++i )
        {
            WindowState::Ptr ws = childAt( i );
            ws->save( elParent );
        }
    }

    void WindowStateRoot::load( const QDomElement& elParent )
    {
        readChildren( elParent, CTWindows );
    }

    void WindowStateRoot::setMode(Mode* mode)
    {
        mMode = mode;
    }

    Mode* WindowStateRoot::mode()
    {
        return mMode;
    }

    WindowStateRoot* WindowStateRoot::root()
    {
        return this;
    }

    void WindowStateRoot::setReadOnly(bool ro)
    {
        mIsReadOnly = ro;
    }

    bool WindowStateRoot::isReadOnly() const
    {
        return mIsReadOnly;
    }

}
