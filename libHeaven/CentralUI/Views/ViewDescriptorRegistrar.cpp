/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#include "libHeavenActions/DynamicActionMerger.hpp"

#include "View.hpp"
#include "ViewDescriptorRegistrar.hpp"

namespace Heaven
{

    /**
     * @internal
     * @class       ViewDescriptor::Registrar
     *
     * @brief       Internal class to manage registered ViewDescriptors and the Views-Menu
     *
     */

    ViewDescriptor::Registrar::Registrar()
    {
        setupActions(this);
        damViewsMerger->setMode(DAMergerAdvancedList);
        sSelf = this;
    }

    ViewDescriptor::Registrar& ViewDescriptor::Registrar::self()
    {
        if (sSelf == NULL) {
            new Registrar();
        }
        return *sSelf;
    }

    ViewDescriptor::Registrar* ViewDescriptor::Registrar::sSelf = NULL;

    void ViewDescriptor::Registrar::insert(const ViewIdentifier& id, ViewDescriptor* vd)
    {
        mDescriptors.insert(id, vd);
        rebuildMerger();
    }

    void ViewDescriptor::Registrar::remove(const ViewIdentifier& id)
    {
        mDescriptors.remove(id);
        rebuildMerger();
    }

    ViewDescriptor* ViewDescriptor::Registrar::get(const ViewIdentifier& id) const
    {
        return mDescriptors.value(id, NULL);
    }

    void ViewDescriptor::Registrar::viewClosed(View* view)
    {
        ViewIdentifier id = view->identifier();
        ViewDescriptor* vd = get(id);

        if (vd) {
            QAction* aa = vd->activatorAction();
            aa->setChecked(false);

            vd->setViewClosed();
        }
    }

    void ViewDescriptor::Registrar::viewOpened(View* view)
    {
        ViewIdentifier id = view->identifier();
        ViewDescriptor* vd = get(id);

        if (vd) {
            QAction* aa = vd->activatorAction();
            aa->setChecked(true);
        }
    }

    DynamicActionMerger* ViewDescriptor::Registrar::actionMerger()
    {
        return damViewsMerger;
    }

    void ViewDescriptor::Registrar::rebuildMerger()
    {
        Q_ASSERT(damViewsMerger);
        damViewsMerger->clear();

        QMap<ViewIdentifier, ViewDescriptor*> descriptors;

        foreach (ViewIdentifier id, mDescriptors.keys()) {
            descriptors.insert(id, mDescriptors[id]);
        }

        foreach (ViewIdentifier id, descriptors.keys()) {
            ViewDescriptor* vd = get(id);
            if (vd) {
                damViewsMerger->addAction(vd->activatorAction());
            }
        }
    }

}
