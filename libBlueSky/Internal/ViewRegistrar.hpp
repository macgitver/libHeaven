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

#ifndef HEAVEN_VIEW_DESCRIPTOR_REGISTRAR_HPP
#define HEAVEN_VIEW_DESCRIPTOR_REGISTRAR_HPP

#include <QHash>
#include <QObject>

#include "libBlueSky/ViewDescriptor.hpp"

#include "hic_ViewsMergerActions.h"

namespace BlueSky {

    namespace Internal {

        class ViewRegistrar : public QObject, public ViewsMergerActions
        {
            Q_OBJECT
        private:
            ViewRegistrar();

        public:
            static ViewRegistrar& self();

        public:
            void remove(const ViewIdentifier& id);
            void insert(const ViewIdentifier& id, ViewDescriptor* vd);
            ViewDescriptor* get(const ViewIdentifier& id) const;

            void viewClosed(View* view);
            void viewOpened(View* view);

            Heaven::DynamicActionMerger* actionMerger();

        private:
            void rebuildMerger();

        private slots:
            void onActivatorAction(bool triggered);

        private:
            static ViewRegistrar* sSelf;
            QHash<ViewIdentifier, ViewDescriptor*> mDescriptors;
            QHash<QAction*, ViewDescriptor*> mDescriptorsByActivator;
        };

    }

}

#endif
