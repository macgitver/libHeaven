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

#include <QMap>
#include <QStringBuilder>

#include "CentralUI/Views/ViewIdentifier.hpp"

#include "CentralUI/Contexts/ContextKeys.hpp"

namespace Heaven
{

    class ContextKeys::Data : public QSharedData
    {
    public:
        Data( const ViewIdentifier& _viewId ) : viewId( _viewId ) {}

    public:
        QMap< QString, QString > keys;
        const ViewIdentifier     viewId;
    };

    ContextKeys::ContextKeys()
    {
    }

    ContextKeys::ContextKeys( const ViewIdentifier& viewId )
    {
        d = new Data( viewId );
    }

    ContextKeys::ContextKeys( const ContextKeys& other )
        : d( other.d )
    {
    }

    ContextKeys::~ContextKeys()
    {
    }

    ContextKeys& ContextKeys::operator=( const ContextKeys& other )
    {
        d = other.d;
        return *this;
    }

    bool ContextKeys::operator==( const ContextKeys& other ) const
    {
        return (d == other.d ) ||
                ( d->viewId == other.d->viewId &&
                  d->keys == other.d->keys );
    }

    bool ContextKeys::isValid() const
    {
        return !!d; // = bool::operator!( QExpli...::operator!() )
    }

    QString ContextKeys::toString() const
    {
        if( !isValid() )
        {
            return QString();
        }

        QString s = d->viewId.toString() % QChar( L':' );

        foreach( QString keyName, d->keys.keys() )
        {
            s = s % keyName % QChar( L'=' ) % d->keys[ keyName ] % QChar( L';' );
        }

        return s;
    }

    ViewIdentifier ContextKeys::viewId() const
    {
        return isValid() ? d->viewId : ViewIdentifier();
    }

    QString ContextKeys::operator[]( const char* pszKey ) const
    {
        return get( pszKey );
    }

    QString ContextKeys::operator[]( const QString& key ) const
    {
        return get( key );
    }

    QString ContextKeys::get( const char* pszKey ) const
    {
        return get( QString::fromLatin1( pszKey ) );
    }

    QString ContextKeys::get( const QString& key ) const
    {
        if( isValid() )
        {
            return d->keys.value( key, QString() );
        }
        return QString();
    }

    void ContextKeys::set( const char* pszKey, const QString& value )
    {
        set( QString::fromLatin1( pszKey ), value );
    }

    void ContextKeys::set( const QString& key, const QString& value )
    {
        Q_ASSERT( isValid() );
        d->keys[ key ] = value;
    }

}
