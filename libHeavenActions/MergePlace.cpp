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

#include "libHeavenActions/MergePlacePrivate.hpp"
#include "libHeavenActions/UiManager.hpp"
#include "libHeavenActions/MergesManager.hpp"

namespace Heaven
{

    MergePlacePrivate::MergePlacePrivate( MergePlace* owner )
        : UiObjectPrivate( owner )
    {
    }

    MergePlacePrivate::~MergePlacePrivate()
    {
        if( !mName.isEmpty() )
        {
            //MergesManager::self()->removeMergePlace( this );
        }
    }

    UiObjectTypes MergePlacePrivate::type() const
    {
        return MergePlaceType;
    }

    MergePlace::MergePlace( QObject* parent )
        : UiObject( parent, new MergePlacePrivate( this ) )
    {
    }

    void MergePlace::setName( const QByteArray& name )
    {
        UIOD(MergePlace);
        d->mName = name;
    }

    QByteArray MergePlace::name() const
    {
        UIOD(const MergePlace);
        return d->mName;
    }

}

