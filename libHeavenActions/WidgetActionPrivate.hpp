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

#ifndef MGV_HEAVEN_WIDGET_ACTION_PRIVATE_H
#define MGV_HEAVEN_WIDGET_ACTION_PRIVATE_H

#include <QSet>

#include "libHeavenActions/WidgetAction.hpp"
#include "libHeavenActions/UiObjectPrivate.hpp"

namespace Heaven
{

    class WidgetActionWrapper;

    class WidgetActionPrivate : public UiObjectPrivate
    {
        Q_OBJECT
    public:
        WidgetActionPrivate( WidgetAction* owner );
        ~WidgetActionPrivate();

    public:
        UiObjectTypes type() const;

    public:
        WidgetActionWrapper* wrapper();

    private:
        WidgetActionWrapper*    mWrapper;
    };

}

#endif
