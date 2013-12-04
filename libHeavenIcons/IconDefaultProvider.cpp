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

#include <QFile>
#include <QStringBuilder>
#include <QPixmap>

#include "libHeavenIcons/IconDefaultProvider.hpp"
#include "libHeavenIcons/IconDefaultProviderPrivate.hpp"

namespace Heaven
{

    IconDefaultProviderPrivate::IconDefaultProviderPrivate()
    {
    }

    IconDefaultProvider::IconDefaultProvider()
    {
        d = new IconDefaultProviderPrivate;
        addSearchPath( QLatin1String( ":/Heaven" ) );
    }

    IconDefaultProvider::~IconDefaultProvider()
    {
        delete d;
    }

    int IconDefaultProvider::baseCacheCost() const
    {
        return 1;
    }

    QString IconDefaultProvider::name() const
    {
        return QString();
    }

    Icon IconDefaultProvider::provide( const IconRef& ref )
    {
        QPixmap pix;

        if( !ref.isValid() )
        {
            return Icon();
        }

        foreach( QString path, d->searchPaths )
        {
            // We hard-code PNG here, since we want it to _work_ now. Further, if it's hard coded
            // here, that also means: It's not embeded in textual representation of Icon-Refs.
            // Which in turn allows us to change it later on without hassle...

            // As we want to change this now: We're now by default searching for a svg, if that
            // fails, we fallback to png
            QString baseName = path % QLatin1String( "/" ) % ref.text();
            QString fName;

            fName = baseName % QLatin1Literal(".svg");
            if (QFile::exists(fName)) {
                pix = QPixmap(fName, "svg");
                if (!pix.isNull()) {
                    break;
                }
            }

            fName = baseName % QLatin1Literal(".png");
            if (QFile::exists(fName)) {
                pix = QPixmap(fName);
                break;
            }
        }

        if( pix.isNull() )
        {
            return Icon();
        }

        Icon icon( ref, pix );

        if( ref.hasSubReference() )
        {
            IconProvider* ip = ref.provider();
            Q_ASSERT( ip );
            icon = ip->applyTo( ref.subReference(), icon );
        }

        return icon;
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
