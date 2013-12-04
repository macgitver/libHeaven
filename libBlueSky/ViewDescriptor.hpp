/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef HEAVEN_VIEW_DESCRIPTOR_HPP
#define HEAVEN_VIEW_DESCRIPTOR_HPP

#include <QObject>

#include "libBlueSky/libBlueSkyAPI.hpp"
#include "libBlueSky/Views.hpp"

class QAction;

namespace Heaven {

    class DynamicActionMerger;

}

namespace BlueSky{

    namespace Internal {

        class ViewRegistrar;

    }

    class View;

    class HEAVEN_BLUESKY_API ViewDescriptor : public QObject
    {
        Q_OBJECT
    public:
        friend class Internal::ViewRegistrar;
        typedef View* (*CreatorFunc)( );

    public:
        ViewDescriptor(const ViewIdentifier& id, const QString& displayName, CreatorFunc creator);
        ~ViewDescriptor();

    public:
        QAction* activatorAction();
        QString displayName() const;
        ViewIdentifier identifier() const;
        ViewDescriptor::CreatorFunc creatorFunc() const;

        void unregister();
        View* createView() const;

    public:
        static Heaven::DynamicActionMerger* actionMerger();
        static void mergeViewsMenu(const QByteArray& mergePlace);
        static ViewDescriptor* get( const ViewIdentifier& id );

    private:
        void setViewClosed();
        void createActivatorAction();

    private:
        void onActivatorAction(bool triggered);

    private:
        const ViewIdentifier    mIdentifier;
        const QString           mDisplayName;
        const CreatorFunc       mCreatorFunc;
        mutable View*           mCreatedView;
        QAction*                mActivatorAction;
    };

}

#endif
