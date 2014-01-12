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

#include <QIcon>

#include "libHeavenIcons/IconManager.hpp"
#include "libHeavenIcons/IconProvider.hpp"
#include "libHeavenIcons/IconDefaultProvider.hpp"

#include "libHeavenIcons/IconManagerPrivate.hpp"
#include "libHeavenIcons/IconPrivate.hpp"

namespace Heaven
{

    /**
     * @class       IconManager
     * @brief       Singleton that manages all icons
     *
     * As a user of libHeaven, you ususally don't need to deal with the IconManager directly. Use
     * the IconRef and Icon classes for loading icons.
     *
     */

    IconManager::IconManager()
    {
        d = new IconManagerPrivate;
        d->cache.setMaxCost( 500 );
        d->defaultProvider = new IconDefaultProvider;
        d->providers.append( d->defaultProvider );
    }

    IconManager::~IconManager()
    {
        qDeleteAll( d->providers ); // includes the default provider.
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

    /**
     * @brief       Access the default provider
     *
     * @return      C++ pointer to the default icon provider.
     *
     */
    IconDefaultProvider* IconManager::defaultProvider() const
    {
        return d->defaultProvider;
    }

    /**
     * @brief       Lookup an icon provider by its name
     *
     * @param[in]   name    Name of the icon provider to lookup.
     *
     * @return      A C++ pointer to the found IconProvider or `NULL` if none was found.
     *
     * @see         registerProvider(), unregisterProvider()
     */
    IconProvider* IconManager::provider( const QString& name ) const
    {
        for( int i = 0; i < d->providers.count(); ++i )
        {
            IconProvider* ip = d->providers.at( i );
            if( ip && ip->name() == name )
            {
                return ip;
            }
        }

        return NULL;
    }

    /**
     * @brief       Register an icon provider
     *
     * @param[in]   provider    Pointer to the provider to register
     *
     * The icon manager takes ownership of the provider. It will be deleted when it is no longer in
     * use.
     */
    void IconManager::registerProvider( IconProvider* provider )
    {
        d->providers.append( provider );
    }

    /**
     * @brief       Unregister an icon provider
     *
     * @param[in]   provider    Pointer to the provider to unregister
     *
     * It is not allowed to unregister the default icon provider.
     *
     * If the provider is registered, it will be deleted.
     *
     */
    void IconManager::unregisterProvider( IconProvider* provider )
    {
        if( provider == d->defaultProvider )
        {
            return;
        }

        for( int i = 0; i < d->providers.count(); ++i )
        {
            IconProvider* ip = d->providers.at( i );
            if( ip == provider )
            {
                d->providers.removeAt( i );
                delete ip;
                return;
            }
        }
    }

    /**
     * @brief       Load an icon
     *
     * @param[in]   ref     An IconRef that specifies what icon is to load.
     *
     * @return      The icon or an invalid Icon object, if the icon cannot be loaded.
     *
     */
    Icon IconManager::icon( const IconRef& ref )
    {
        Icon i;

        if( !ref.isValid() )
        {
            return i;
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

        i = ip->provide( ref );

        if( i.isValid() )
        {
            d->cache.insert( cryptoHash, new Icon( i ) );
        }

        return i;
    }

}
