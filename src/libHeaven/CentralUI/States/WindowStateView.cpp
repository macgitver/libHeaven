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

#include <QDomElement>

#include "libHeaven/CentralUI/States/WindowStateView.hpp"

namespace Heaven
{

    WindowStateView::WindowStateView( WindowState* parent )
        : WindowState( parent )
    {
    }

    WindowStateView::WindowStateView(WindowState* parent, WindowStateView* cloneFrom)
        : WindowState(parent, cloneFrom)
    {
    }

    WindowStateView::WindowStateView( WindowState* parent, QDomElement& el )
        : WindowState( parent )
    {
        readOrCreateIdentifier( el );
    }

    WindowStateView* WindowStateView::clone(WindowState* toParent)
    {
        return new WindowStateView(toParent, this);
    }

    void WindowStateView::updateConfig()
    {
    }

    void WindowStateView::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "View" ) );
        saveIdentifier( elChild );
        elParent.appendChild( elChild );
    }

    WindowState::Type WindowStateView::type() const
    {
        return WSView;
    }

}
