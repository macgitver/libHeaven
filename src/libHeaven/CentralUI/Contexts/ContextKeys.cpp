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

    /**
     * @class       ContextKeys
     * @brief       Keys identifiying a view-context
     *
     */

    class ContextKeys::Data : public QSharedData
    {
    public:
        Data( const ViewIdentifier& _viewId ) : viewId( _viewId ) {}

    public:
        QMap< QString, QString > keys;
        const ViewIdentifier     viewId;
    };

    /**
     * @brief       Constructor
     *
     * Constructs invalid context keys.
     */
    ContextKeys::ContextKeys()
    {
    }

    /**
     * @brief       Constructor
     *
     * @param[in]   viewId      ViewIdentifier for which this keys should be created.
     *
     * Creates a valid context keys object for the ViewIdentifier @a viewId.
     *
     */
    ContextKeys::ContextKeys( const ViewIdentifier& viewId )
    {
        d = new Data( viewId );
    }

    /**
     * @brief       Constructor ( Copy )
     *
     * @param[in]   other       ContextKeys to copy.
     *
     * Creates a context keys object from another one.
     *
     */
    ContextKeys::ContextKeys( const ContextKeys& other )
        : d( other.d )
    {
    }

    /**
     * @brief       Destructor
     *
     */
    ContextKeys::~ContextKeys()
    {
    }

    /**
     * @brief       Assignment Operator
     *
     * @param[in]   other   ContextKeys to assgin
     *
     * @return      A reference to this.
     *
     * Copys the @a other ContextKeys into this object.
     *
     */
    ContextKeys& ContextKeys::operator=( const ContextKeys& other )
    {
        d = other.d;
        return *this;
    }

    /**
     * @brief       Compare Equality Operator
     *
     * @param[in]   other   ContextKeys to compare against
     *
     * @return      `true` if this Context Keys object is equal to the @a other context keys object,
     *              `false` otherwise. Two context keys objects are considered equal, if they are
     *              actually the same object or thier viewName and all key/values are identical.
     */
    bool ContextKeys::operator==( const ContextKeys& other ) const
    {
        return (d == other.d ) ||
                ( d->viewId == other.d->viewId &&
                  d->keys == other.d->keys );
    }

    /**
     * @brief       Is this context keys object valid
     *
     * @return      `true`, if this context keys objcect is valid, `false` otherwise.
     *
     */
    bool ContextKeys::isValid() const
    {
        return !!d; // = bool::operator!( QExpli...::operator!() )
    }

    /**
     * @brief       Create a readable and hashable string
     *
     * @return      A string that uniquely describes this context keys object
     *
     * An empty string for invalid context keys objects; for valid context key objects: the view
     * identifier followed by a colon followed by an optional list of key value pairs. Each key
     * value pair consists of the key, an equal sign, the value and a semicolon. No escaping is
     * performed. The key value list is in arbitrary, but stable order.
     *
     */
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

    /**
     * @brief       Get the View Identifier
     *
     * @return      View Identifer to which this context keys object applies.
     *
     */
    ViewIdentifier ContextKeys::viewId() const
    {
        return isValid() ? d->viewId : ViewIdentifier();
    }

    /**
     * @brief       Index operator
     *
     * @param[in]   pszKey      Ascii key to fetch
     *
     * @return      The value stored under the key @a pszKey.
     *
     */
    QString ContextKeys::operator[]( const char* pszKey ) const
    {
        return get( pszKey );
    }

    /**
     * @brief       Index operator
     *
     * @param[in]   key     The key to fetch
     *
     * @return      The value stored under the key @a key.
     *
     */
    QString ContextKeys::operator[]( const QString& key ) const
    {
        return get( key );
    }

    /**
     * @brief       Get a value
     *
     * @param[in]   pszKey      Ascii key to fetch
     *
     * @return      The value stored under the key @a pszKey.
     *
     */
    QString ContextKeys::get( const char* pszKey ) const
    {
        return get( QString::fromLatin1( pszKey ) );
    }

    /**
     * @brief       Get a value
     *
     * @param[in]   key     The key to fetch
     *
     * @return      The value stored under the key @a key.
     *
     */
    QString ContextKeys::get( const QString& key ) const
    {
        if( isValid() )
        {
            return d->keys.value( key, QString() );
        }
        return QString();
    }

    /**
     * @brief[in]   Change a value
     *
     * @param[in]   pszKey      Ascii key to change
     *
     * @param[in]   value       The new value to store for the key @a pszKey.
     *
     */
    void ContextKeys::set( const char* pszKey, const QString& value )
    {
        set( QString::fromLatin1( pszKey ), value );
    }

    /**
     * @brief[in]   Change a value
     *
     * @param[in]   key         Key to change
     *
     * @param[in]   value       The new value to store for the key @a key.
     *
     */
    void ContextKeys::set( const QString& key, const QString& value )
    {
        Q_ASSERT( isValid() );
        d->keys[ key ] = value;
    }

}
