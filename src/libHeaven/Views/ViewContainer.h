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

#ifndef HEAVEN_CONTAINER_H
#define HEAVEN_CONTAINER_H

#include <QObject>
#include <QSet>

#include "libHeaven/HeavenApi.hpp"
#include "libHeaven/Views/ViewContainerContent.h"

class QSplitter;

namespace Heaven
{

    class MultiBarContainerWidget;
    class TabWidget;
    class View;

    class HEAVEN_API ViewContainer : public QObject, public ViewContainerContent
    {
        Q_OBJECT
    public:
        enum Type
        {
            Tab             = 1 << 0,
            Splitter        = 1 << 1,
            MultiBar        = 1 << 2
        };

        enum Subtype
        {
            SubTabTop       = 0,
            SubTabBottom    = 1,
            SubTabRight     = 2,
            SubTabLeft      = 3,

            SubSplitVert    = 0,
            SubSplitHorz    = 1
        };

    public:
        ViewContainer( const QString& identifier, Type t, Subtype s, ViewContainer* parent = NULL );
        ~ViewContainer();

    public:
        Type type() const;

        void clear();
        QWidget* containerWidget();

        void insertContainer( int pos, ViewContainer* container );

        ViewContainerContent* take( ViewContainerContent* cc );
        ViewContainerContent* takeAt( int index );
        int indexOf( ViewContainerContent* cc ) const;
        QList< ViewContainerContent* > contents() const;

        void add( ViewContainerContent* content );

        Subtype subtype() const;
        void changeSubtype( Subtype subtype );

    public:
        bool isContainer() const;
        ViewContainer* asContainer();
        QWidget* widget();

    private:
        void setSubtype( Subtype subtype );

    private:
        Type                            mType;
        Subtype                         mSubtype;
        QList< ViewContainerContent* >  mContents;

        union
        {
            QWidget*            mContainerWidget;
            QSplitter*          mSpliterWidget;
            TabWidget*          mTabWidget;
            MultiBarContainerWidget*  mMultiBarContainer;
        };
    };

}

#endif
