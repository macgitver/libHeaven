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

#include "libBlueSky/Views.hpp"
#include "libBlueSky/Internal/ViewRegistrar.hpp"

namespace BlueSky {

    namespace Internal {

        /**
         * @internal
         * @class       ViewRegistrar
         *
         * @brief       Internal class to manage registered ViewDescriptors and the Views-Menu
         *
         */
        ViewRegistrar::ViewRegistrar()
        {
            setupActions(this);
            damViewsMerger->setMode(Heaven::DAMergerAdvancedList);
            sSelf = this;
        }

        ViewRegistrar* ViewRegistrar::sSelf = NULL;

        ViewRegistrar& ViewRegistrar::self() {
            if (!sSelf) new ViewRegistrar;

            Q_ASSERT(sSelf);
            return *sSelf;
        }

        void ViewRegistrar::insert(const ViewIdentifier& id, ViewDescriptor* vd)
        {
            mDescriptors.insert(id, vd);

            QAction* activator = vd->activatorAction();
            mDescriptorsByActivator.insert(activator, vd);
            connect(activator, SIGNAL(triggered(bool)),
                    this, SLOT(onActivatorAction(bool)));

            rebuildMerger();
        }

        void ViewRegistrar::remove(const ViewIdentifier& id)
        {
            ViewDescriptor* vd = mDescriptors.take(id);
            if (!vd) {
                return;
            }

            mDescriptorsByActivator.remove(vd->activatorAction());

            rebuildMerger();
        }

        ViewDescriptor* ViewRegistrar::get(const ViewIdentifier& id) const
        {
            return mDescriptors.value(id, NULL);
        }

        void ViewRegistrar::viewClosed(View* view)
        {
            ViewIdentifier id = view->identifier();
            ViewDescriptor* vd = get(id);

            if (vd) {
                QAction* aa = vd->activatorAction();
                aa->setChecked(false);

                vd->setViewClosed();
            }
        }

        void ViewRegistrar::viewOpened(View* view)
        {
            ViewIdentifier id = view->identifier();
            ViewDescriptor* vd = get(id);

            if (vd) {
                QAction* aa = vd->activatorAction();
                aa->setChecked(true);
            }
        }

        Heaven::DynamicActionMerger* ViewRegistrar::actionMerger()
        {
            return damViewsMerger;
        }

        void ViewRegistrar::rebuildMerger()
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

        void ViewRegistrar::onActivatorAction(bool triggered) {
            QAction* activator = qobject_cast<QAction*>(sender());
            if (!activator) {
                return;
            }

            ViewDescriptor* vd = mDescriptorsByActivator.value(activator, NULL);
            if (!vd) {
                return;
            }

            vd->onActivatorAction(triggered);
        }

    }

}
