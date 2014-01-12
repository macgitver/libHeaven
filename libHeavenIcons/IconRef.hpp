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

#ifndef HAVEN_ICON_REF_HPP
#define HAVEN_ICON_REF_HPP

#include "libHeavenIcons/libHeavenIconsAPI.hpp"

#include <QSharedData>

class QByteArray;
class QString;

namespace Heaven
{

    class Icon;
    class IconProvider;

    class HEAVEN_ICONS_API IconRef
    {
    public:
        IconRef();
        IconRef( const IconRef& other );
        IconRef( IconProvider* provider, const char* pszText, int size );
        IconRef( IconProvider* provider, const QString& text, int size );
        IconRef( const QString& text, int size );
        IconRef( const char* pszText, int size );
        ~IconRef();

        IconRef& operator=( const IconRef& other );
        IconRef& operator=( const QString& text );
        IconRef& operator=( const char* pszText );

        bool operator==( const IconRef& other ) const;

        bool isValid() const;

        QString toString() const;
        static IconRef fromString( const QString& str );
        static IconRef fromString( const char* pszStr );

    public:
        IconProvider* provider() const;
        QString text() const;
        int size() const;
        int numParameters() const;
        QString parameter( int index ) const;

        void setSize( int size );
        void setText( const QString& text );
        void setProvider( IconProvider* provider );

        void appendParam( const QString& text );
        void appendParam( const char* szText );
        void appendParam( const IconRef& other );

        void set( IconProvider* provider, const QString& text, int size );

        bool hasSubReference() const;
        IconRef subReference() const;

        QByteArray cryptoHash() const;
        int numComponents() const;

        Icon icon() const;

    private:
        class Data;
        QExplicitlySharedDataPointer< Data > d;
    };

}

#endif
