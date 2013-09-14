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

#include <QDebug>

#include "libHeaven/Actions/ActionGroupPrivate.hpp"
#include "libHeaven/Actions/ActionPrivate.hpp"
#include "libHeaven/Actions/UiManager.hpp"

namespace Heaven
{

    ActionGroupPrivate::ActionGroupPrivate(ActionGroup *owner)
        : UiContainer(owner)
    {
    }

    ActionGroupPrivate::~ActionGroupPrivate()
    {
    }

    UiObjectTypes ActionGroupPrivate::type() const
    {
        return ActionGroupType;
    }

    QActionGroup* ActionGroupPrivate::groupForParent(QObject* forParent)
    {
        QActionGroup* grp = mCreatedGroups.value(forParent);
        if (grp) {
            return grp;
        }

        grp = new QActionGroup(forParent);

        foreach (UiObjectPrivate* uio, allObjects()) {
            ActionPrivate* ap = qobject_cast< ActionPrivate* >(uio);

            if (ap) {
                grp->addAction(ap->getOrCreateQAction(forParent));
            }
        }

        return grp;
    }


    ActionGroup::ActionGroup(QObject *parent)
        : UiObject(parent, new ActionGroupPrivate(this))
    {
    }

    void ActionGroup::add(Action* action)
    {
        if (action) {

            if (action->group()) {
                qDebug() << "Action" << action->objectName() << "already has a group.";
                return;
            }

            UIOD(ActionGroup);
            d->add(action);

            action->setGroup(this);
        }
    }

    QActionGroup* ActionGroup::groupForParent(QObject *forParent)
    {
        UIOD(ActionGroup);
        return d->groupForParent(forParent);
    }

}
