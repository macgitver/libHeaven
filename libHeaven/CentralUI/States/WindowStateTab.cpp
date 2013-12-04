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

#include "libHeaven/CentralUI/States/WindowStateTab.hpp"

namespace Heaven
{

    static inline Positions parseTabPosition( const QString& s )
    {
        if( s == QLatin1String( "Left" ) )
            return Left;
        if( s == QLatin1String( "Right" ) )
            return Right;
        if( s == QLatin1String( "Bottom" ) )
            return Bottom;

        return Top;
    }

    WindowStateTab::WindowStateTab( WindowState* parent, QDomElement& el )
        : WindowState( parent )
    {
        mPositions = parseTabPosition( el.attribute( QLatin1String( "Pos" ),
                                                     QLatin1String( "Top" ) ) );

        readOrCreateIdentifier( el );
        readChildren( el, CTContainers | CTViews );
    }

    WindowStateTab::WindowStateTab( WindowState* parent )
        : WindowState( parent )
    {
    }

    void WindowStateTab::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Tab" ) );
        elParent.appendChild( elChild );

        const char* s = NULL;
        switch( mPositions )
        {
        case Bottom:   s = "Bottom";   break;
        case Top:      s = "Top";      break;
        case Left:     s = "Left";     break;
        case Right:    s = "Right";    break;
        default:                       break;
        }

        if( s )
        {
            elChild.setAttribute( QLatin1String( "Pos" ), QLatin1String( s ) );
        }

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

    void WindowStateTab::updateConfig()
    {
        WindowState::updateConfig();
    }

    WindowState::Type WindowStateTab::type() const
    {
        return WSTab;
    }

    void WindowStateTab::setTabPosition( Positions pos )
    {
        mPositions = pos;
    }

    Positions WindowStateTab::tabPosition() const
    {
        return mPositions;
    }

}
