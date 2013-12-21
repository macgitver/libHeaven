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

#ifndef HEAVEN_SPLITTER_CONTAINER_WIDGET_HPP
#define HEAVEN_SPLITTER_CONTAINER_WIDGET_HPP

#include "libHeaven/Heaven.hpp"
#include "libHeaven/CentralUI/ContainerWidgets/ContainerWidget.hpp"
#include "libHeaven/Widgets/MiniSplitter.hpp"

namespace Heaven
{

    class SplitterContainerWidget : public ContainerWidget
    {
        Q_OBJECT
    public:
        SplitterContainerWidget( const ViewIdentifier& identifier, bool isVertical = false );

    public:
        int insert( int index, AbstractViewWidget* view );
        AbstractViewWidget* takeAt( int index );
        int indexOf( AbstractViewWidget* widget ) const;
        AbstractViewWidget* widget( int index );
        int count() const;

    public:
        ContainerTypes containerType() const;

    public:
        void setVertical( bool vert );
        bool isVertical() const;

    public:
        QList<int> transformedSizes();
        void setTransformedSizes(const QList<int> sizes);

    private:
        MiniSplitter* mSplitter;
    };

}

#endif
