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

#ifndef HEAVEN_WINDOW_HPP
#define HEAVEN_WINDOW_HPP

#include <QObject>

class QDomElement;

namespace Heaven
{

    class IStoredState
    {
    public:
        virtual void saveState( QDomElement& elContainer ) const = 0;
        virtual void loadState( const QDomElement& elContainer ) = 0;
    };

}

Q_DECLARE_INTERFACE( Heaven::IStoredState, "org.macgiter.heaven/0.1" )

#endif
