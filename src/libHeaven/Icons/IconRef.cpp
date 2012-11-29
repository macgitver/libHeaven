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

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QStringList>

#include "libHeaven/Icons/Icon.hpp"
#include "libHeaven/Icons/IconRef.hpp"
#include "libHeaven/Icons/IconManager.hpp"

namespace Heaven
{

    /**
     * @class       IconRef
     * @brief       Reference to an icon
     *
     * An IconRef object holds a reference to an icon. This can be complex, as it is actually a path
     * that describes how the final icon is composed of others.
     *
     * This path is represented by a string. Such a string can be placed into a HID file. In its
     * simplest form, it just names the icon: `#R2D2` (Meaning simply: Load an icon named R2D2)
     *
     * A more advanced icon ref would look like: `mime#text/plain:ovl#save$2-1-1` This would mean:
     * Ask the __mime__ IconProvider for an icon named __text/plain__. Feed that icon to the __ovl__
     * IconProvider, which is given the name __save__ and a parameter __2-1-1__.
     *
     * The result being: An icon for the mime type _text/plain_ will be created and overlayed with a
     * _save_ icon. The parameter 2-1-1 is interpreted as: divide the icon space by 2 and place the
     * overlay in box 1|1 (zero based), thus into the bottom-right quarter.
     */

    class IconRef::Data : public QSharedData
    {
    public:
        IconProvider*       provider;
        QString             text;
        int                 size;
        QStringList         parameters;
        IconRef             refParam;
        mutable QByteArray  cryptoHash;
    };

    IconRef::IconRef()
    {
    }

    IconRef::IconRef( const IconRef& other )
    {
        d = other.d;
    }

    IconRef::IconRef( IconProvider* provider, const char* pszText, int size )
    {
        set( provider, QString::fromUtf8( pszText ), size );
    }

    IconRef::IconRef( IconProvider* provider, const QString& text, int size )
    {
        set( provider, text, size );
    }

    IconRef::IconRef( const QString& text, int size )
    {
        set( NULL, text, size );
    }

    IconRef::IconRef( const char* pszText, int size )
    {
        set( NULL, QString::fromUtf8( pszText ), size );
    }

    IconRef::~IconRef()
    {
    }

    IconRef& IconRef::operator=( const IconRef& other )
    {
        d = other.d;
        return * this;
    }

    IconRef& IconRef::operator=( const QString& text )
    {
        return *this = IconRef( text, -1 );
    }

    IconRef& IconRef::operator=( const char* pszText )
    {
        return *this = IconRef( pszText, -1 );
    }

    bool IconRef::operator==( const IconRef& other ) const
    {
        if( d.constData() == other.d.constData() )
            return true;

        if( !d || !other.d )
            return false;

        return d->provider == other.d->provider &&
                d->text == other.d->text &&
                d->size == other.d->size &&
                d->parameters == other.d->parameters &&
                d->refParam == other.d->refParam;
    }

    bool IconRef::isValid() const
    {
        return d.data() != NULL;
    }

    QString IconRef::toString() const
    {
        return QString();
    }

    IconRef IconRef::fromString( const QString& str )
    {
        return IconRef();
    }

    IconRef IconRef::fromString( const char* pszStr )
    {
        return fromString( QString::fromUtf8( pszStr ) );
    }

    IconProvider* IconRef::provider() const
    {
        Q_ASSERT( d );
        return d->provider;
    }

    QString IconRef::text() const
    {
        Q_ASSERT( d );
        return d->text;
    }

    int IconRef::size() const
    {
        Q_ASSERT( d );
        return d->size;
    }

    int IconRef::numParameters() const
    {
        Q_ASSERT( d );
        return d->parameters.count() + ( d->refParam.isValid() ? 1 : 0 );
    }

    QString IconRef::parameter( int index ) const
    {
        Q_ASSERT( d );
        if( index == d->parameters.count() )
        {
            return d->refParam.toString();
        }

        return d->parameters.at( index );
    }

    void IconRef::setSize( int size )
    {
        Q_ASSERT( d );
        d->size = size;
        d->cryptoHash = QByteArray();
    }

    void IconRef::setText( const QString& text )
    {
        Q_ASSERT( d );
        d->text = text;
        d->cryptoHash = QByteArray();
    }

    void IconRef::setProvider( IconProvider* provider )
    {
        Q_ASSERT( d );
        d->provider = provider;
        d->cryptoHash = QByteArray();
    }

    void IconRef::appendParam( const QString& text )
    {
        Q_ASSERT( d );
        d->parameters.append( text );
        d->cryptoHash = QByteArray();
    }

    void IconRef::appendParam( const char* szText )
    {
        appendParam( QString::fromUtf8( szText ) );
    }

    void IconRef::appendParam( const IconRef& other )
    {
        Q_ASSERT( d );
        Q_ASSERT( !d->refParam.isValid() );
        d->refParam = other;
        d->cryptoHash = QByteArray();
    }

    void IconRef::set( IconProvider* provider, const QString& text, int size )
    {
        d = new Data;
        d->provider = provider;
        d->text = text;
        d->size = size;
        d->cryptoHash = QByteArray();
    }

    QByteArray IconRef::cryptoHash() const
    {
        if( !d )
        {
            return QByteArray();
        }

        if( d->cryptoHash.isNull() )
        {
            d->cryptoHash = QCryptographicHash::hash( toString().toUtf8(),
                                                      QCryptographicHash::Sha1 );
        }

        return d->cryptoHash;
    }

    int IconRef::numComponents() const
    {
        if( !d )
        {
            return 0;
        }

        if( d->refParam.isValid() )
        {
            return 1 + d->refParam.numComponents();
        }

        return 1;
    }

    /**
     * @brief       Load the icon
     *
     * @return      The loaded icon or an invalid Icon object, if the icon cannot be loaded.
     *
     */
    Icon IconRef::icon() const
    {
        return IconManager::self().icon( *this );
    }

    bool IconRef::hasSubReference() const
    {
        return d && d->refParam.isValid();
    }

    IconRef IconRef::subReference() const
    {
        return d ? d->refParam : IconRef();
    }

}
