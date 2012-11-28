/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QIcon>

#include "Icons/IconManager.hpp"
#include "Icons/IconProvider.hpp"
#include "Icons/IconDefaultProvider.hpp"

#include "Icons/IconManagerPrivate.hpp"
#include "Icons/IconPrivate.hpp"

namespace Heaven
{

    IconManager::IconManager()
    {
        d = new IconManagerPrivate;
        d->cache.setMaxCost( 500 );
        d->defaultProvider = new IconDefaultProvider;
    }

    IconManager::~IconManager()
    {
        delete d;
    }

    IconManager* IconManager::sSelf = NULL;

    IconManager& IconManager::self()
    {
        if( sSelf == NULL )
        {
            sSelf = new IconManager;
        }

        return *sSelf;
    }

    Icon IconManager::icon( const IconRef& ref )
    {
        if( !ref.isValid() )
        {
            return Icon();
        }

        QByteArray cryptoHash = ref.cryptoHash();

        if( d->cache.contains( cryptoHash ) )
        {
            return * d->cache.object( cryptoHash );
        }

        IconProvider* ip = ref.provider();
        if( !ip )
        {
            ip = d->defaultProvider;
        }

        Icon i = ip->provide( ref );
        if( !i.isValid() )
        {
            return Icon();
        }

        d->cache.insert( cryptoHash, new Icon( i ) );
        return i;
    }

}
