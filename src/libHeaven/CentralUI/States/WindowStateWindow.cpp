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

#include "libHeaven/CentralUI/States/WindowStateWindow.hpp"

namespace Heaven
{

    WindowStateWindow::WindowStateWindow(WindowState* parent)
        : WindowState(parent)
    {
    }

    WindowStateWindow::WindowStateWindow(WindowState* parent, WindowStateWindow* cloneFrom)
        : WindowState(parent, cloneFrom)
    {
    }

    WindowStateWindow::WindowStateWindow(WindowState* parent, QDomElement& elParent)
        : WindowState(parent)
    {
        readOrCreateIdentifier(elParent);
        readChildren(elParent, CTContainers);
    }

    WindowState::Type WindowStateWindow::type() const
    {
        return WSWindow;
    }

    WindowStateWindow* WindowStateWindow::clone(WindowState* toParent)
    {
        return new WindowStateWindow(toParent, this);
    }

    void WindowStateWindow::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Window" ) );
        elParent.appendChild( elChild );

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

}
