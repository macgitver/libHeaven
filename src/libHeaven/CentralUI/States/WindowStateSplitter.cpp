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

#include "libHeaven/CentralUI/States/WindowStateSplitter.hpp"

namespace Heaven
{

    static inline bool parseOrient( const QString& s )
    {
        return s == QLatin1String( "Vert" );
    }

    WindowStateSplitter::WindowStateSplitter( WindowState* parent )
        : WindowState( parent )
    {
    }

    WindowStateSplitter::WindowStateSplitter( WindowState* parent, QDomElement& el )
        : WindowState( parent )
    {
        mVertical = parseOrient( el.attribute( QLatin1String( "Orient" ),
                                               QLatin1String( "Vert" ) ) );

        readOrCreateIdentifier( el );
        readChildren( el, CTContainers | CTViews );
    }

    void WindowStateSplitter::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Split" ) );
        elParent.appendChild( elChild );

        elChild.setAttribute( QLatin1String( "Orient" ),
                              QLatin1String( mVertical ? "Vert" : "Horz" ) );

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

    void WindowStateSplitter::updateConfig()
    {
        WindowState::updateConfig();
    }

    WindowState::Type WindowStateSplitter::type() const
    {
        return WSSplitter;
    }

    void WindowStateSplitter::setVertical( bool value )
    {
        mVertical = value;
    }

    bool WindowStateSplitter::isVertical() const
    {
        return mVertical;
}

}
