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

#include <QWidget>

#include "libHeaven/Actions/UiManager.h"

namespace Heaven
{

    UiManager::UiManager()
        : QObject()
    {
    }

    UiManager::~UiManager()
    {
    }

    UiManager* UiManager::sSelf = NULL;

    UiManager* UiManager::self()
    {
        if( sSelf == NULL )
        {
            sSelf = new UiManager;
        }

        return sSelf;
    }

    void UiManager::addUiObject( UiObjectPrivate* uio )
    {
        mUioUsage.insert( uio, QSet< UiObjectPrivate* >() );
    }

    void UiManager::delUiObject( UiObjectPrivate* uio )
    {
        QSet< UiObjectPrivate* > usage = mUioUsage.value( uio );
        mUioUsage.remove( uio );

        foreach( UiObjectPrivate* used, usage )
        {
            Q_UNUSED( used );
        }
    }

}
