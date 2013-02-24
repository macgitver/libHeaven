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

#include "Mode.h"
#include "ModePrivate.h"
#include "Heaven.hpp"

namespace Heaven
{

    Mode::Mode( const QString& name, const QDomElement& elParent )
        : d( new ModePrivate )
    {
        d->mName = name;
        d->mRoot = new WindowStateRoot( elParent );
    }

    Mode::~Mode()
    {
        delete d;
    }

    QString Mode::name() const
    {
        return d->mName;
    }

    void Mode::setName( const QString& name )
    {
        if( name != d->mName )
        {
            d->mName = name;
            emit nameChanged( name );
        }
    }

    void Mode::activate()
    {
        // Nothing to do here, right? Everything must be done in Application
    }

    void Mode::deactivate()
    {
        Q_ASSERT( d->mRoot );

    }

    WindowStateRoot::Ptr Mode::state() const
    {
        return d->mRoot;
    }

}
