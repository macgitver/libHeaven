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

#ifndef MGV_HEAVEN_ACTION_GROUP_HPP
#define MGV_HEAVEN_ACTION_GROUP_HPP

#include "libHeaven/Actions/UiObject.hpp"

class QActionGroup;

namespace Heaven
{

    class Action;

    class HEAVEN_API ActionGroup : public UiObject
    {
        Q_OBJECT
    public:
        ActionGroup( QObject* parent );

    public:
        void add(Action* action);

    public:
        QActionGroup* groupForParent(QObject* forParent);
    };

}

#endif
