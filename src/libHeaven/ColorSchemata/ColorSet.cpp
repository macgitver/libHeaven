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

#include <QStringBuilder>

#include "libHeaven/ColorSchemata/ColorSet.hpp"

namespace Heaven
{

    ColorDef::ColorDef()
    {
        mId = -1;
    }


    ColorDef::ColorDef( ColorId id, const QByteArray& name, const QString& translatedName,
                        int sortOrder )
    {
        mId = id;
        mSortOrder = sortOrder;
        mName = name;
        mTranslatedName = translatedName;
    }

    ColorDef::ColorDef( const ColorDef& other )
        : mId( other.mId )
        , mSortOrder( other.mSortOrder )
        , mName( other.mName )
        , mTranslatedName( other.mTranslatedName )
    {
    }

    ColorId ColorDef::id() const
    {
        return mId;
    }

    QByteArray ColorDef::name() const
    {
        return mName;
    }

    QString ColorDef::translatedName() const
    {
        return mTranslatedName;
    }

    int ColorDef::sortOrder() const
    {
        return mSortOrder;
    }

    ColorSet::ColorSet( ColorSet* parent )
    {
        mParent = parent;
    }

    ColorSet::ColorSet()
        : mParent( NULL )
    {
    }

    ColorSet::~ColorSet()
    {
        foreach( ColorSet* set, mChildren )
        {
            delete set;
        }
    }

    ColorSet* ColorSet::child( const QByteArray& name ) const
    {
        return mChildren.value( name, NULL );
    }

    QList< ColorSet* > ColorSet::children() const
    {
        return mChildren.values();
    }

    QHash< QByteArray, ColorDef > ColorSet::colorDefs() const
    {
        return mColorIds;
    }

    ColorSet* ColorSet::addSet( const QByteArray& name, const QString& translatedName,
                                int sortOrder )
    {
        ColorSet* set = mChildren.value( name, NULL );
        if( set )
        {
            return set;
        }

        if( sortOrder == -1 )
        {
            sortOrder = mChildren.count();
        }

        set = new ColorSet( this );
        set->mName = name;
        set->mSortOrder = sortOrder;
        set->mTranslatedName = translatedName;

        mChildren.insert( name, set );
        return set;
    }

    QByteArray ColorSet::path() const
    {
        return mParent
                ? mParent->path() % '/' % mName
                : mName;
    }

    QByteArray ColorSet::name() const
    {
        return mName;
    }

    QString ColorSet::translatedName() const
    {
        return mTranslatedName;
    }

    int ColorSet::sortOrder() const
    {
        return mSortOrder;
    }

    ColorId ColorSet::findId( const QList< QByteArray >& paths ) const
    {
        const ColorSet* set = this;
        for( int i = 0; i < paths.count() - 1; i++ )
        {
            const ColorSet* set2 = mChildren.value( paths[ i ], NULL );
            if( !set2 )
            {
                return -1;
            }
            set = set2;
        }

        QByteArray last = paths.last();
        if( !set->mColorIds.contains( last ) )
        {
            return -1;
        }

        return set->mColorIds[ last ].id();
    }

    bool ColorSet::addColor( ColorId id, const QByteArray& name, const QString& translatedName,
                             int sortOrder )
    {
        if( mColorIds.contains( name ) )
        {
            return false;
        }

        if( sortOrder == -1 )
        {
            sortOrder = mColorIds.count();
        }

        mColorIds.insert( name, ColorDef( id, name, translatedName, sortOrder ) );
        return true;
    }


}
