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

#include <QAction>

#include "libHeavenActions/WidgetActionPrivate.hpp"
#include "libHeavenActions/WidgetActionWrapper.hpp"

namespace Heaven
{

    WidgetActionPrivate::WidgetActionPrivate( WidgetAction* owner )
        : UiObjectPrivate( owner )
        , mWrapper( NULL )
    {
    }

    WidgetActionPrivate::~WidgetActionPrivate()
    {
    }

    UiObjectTypes WidgetActionPrivate::type() const
    {
        return WidgetActionType;
    }

    WidgetActionWrapper* WidgetActionPrivate::wrapper()
    {
        if( !mWrapper )
        {
            mWrapper = new WidgetActionWrapper( this );
        }

        return mWrapper;
    }

    WidgetAction::WidgetAction( QObject* parent )
        : UiObject( parent, new WidgetActionPrivate( this ) )
    {
    }

}
