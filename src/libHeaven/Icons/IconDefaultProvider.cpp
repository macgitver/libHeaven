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

#include "libHeaven/Icons/IconDefaultProvider.hpp"
#include "libHeaven/Icons/IconDefaultProviderPrivate.hpp"

namespace Heaven
{

    IconDefaultProviderPrivate::IconDefaultProviderPrivate()
    {
    }

    IconDefaultProvider::IconDefaultProvider()
    {
        d = new IconDefaultProviderPrivate;
    }

    IconDefaultProvider::~IconDefaultProvider()
    {
        delete d;
    }

    QString IconDefaultProvider::name() const
    {
        return QString();
    }

    Icon IconDefaultProvider::provide( const IconRef& ref )
    {
        return Icon();
    }

    void IconDefaultProvider::addSearchPath( const QString& path )
    {
        d->searchPaths.append( path );
    }

    void IconDefaultProvider::delSearchPath( const QString& path )
    {
        d->searchPaths.removeAll( path );
    }

}
