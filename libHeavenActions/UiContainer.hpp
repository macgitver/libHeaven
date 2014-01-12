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

#ifndef MGV_HEAVEN_UICONTAINER_H
#define MGV_HEAVEN_UICONTAINER_H

#include <QList>

class QMenu;
class QToolBar;
class QMenuBar;

#include "libHeavenActions/UiObjectPrivate.hpp"

namespace Heaven
{

    class MenuPrivate;
    class UiObject;

    class UiContainer : public UiObjectPrivate
    {
        friend class MenuPrivate;
        Q_OBJECT
    protected:
        UiContainer( QObject* owner );
        ~UiContainer();

    public:
        void add( UiObject* uio );
        void add( UiObjectPrivate* uio );
        void remove( UiObjectPrivate* uio );

    public:
        bool isContainerDirty() const;
        virtual void setContainerDirty( bool value = true );
        virtual int priority() const;

        virtual bool mergeInto( QMenu* menu );
        virtual bool mergeInto( QMenuBar* menuBar );
        virtual bool mergeInto( QToolBar* toolBar );

        QList< UiContainer* > pathTo( UiObjectPrivate* child );

        bool hasDynamicContent() const;

    protected:
        int numObjects() const;
        UiObjectPrivate* objectAt( int index );
        QList< UiObjectPrivate* > allObjects() const;

    private:
        bool                        mDirty;
        QList< UiObjectPrivate* >   mContent;
    };

}

#endif
