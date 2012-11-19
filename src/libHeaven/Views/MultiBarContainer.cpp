/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QStackedWidget>

#include "libHeaven/Views/MultiBarContainer.hpp"

#include "libHeaven/Widgets/MultiBar.hpp"

namespace Heaven
{

    static inline bool isVertical( MultiBarContainer::BarPos pos )
    {
        return ( pos == MultiBarContainer::West ) || ( pos == MultiBarContainer::East );
    }

    class MultiBarContainerPrivate
    {
    public:
        MultiBarContainerPrivate();

    public:
        void relayout();

    public:
        MultiBarContainer*              owner;
        MultiBarContainer::BarPos       barPos;
        QList< ViewContainerContent* >  views;
        ViewContainerContent*           active;
        QStackedWidget*                 stack;
        MultiBar*                       toolingBar;
        MultiBar*                       viewsBar;
    };

    MultiBarContainerPrivate::MultiBarContainerPrivate()
    {
        owner = NULL;
        barPos = MultiBarContainer::North;
        active = NULL;
        toolingBar = viewsBar = NULL;
    }
    
    MultiBarContainer::MultiBarContainer()
    {
        d = new MultiBarContainerPrivate;
    }

    MultiBarContainer::~MultiBarContainer()
    {
        delete d;
    }

    MultiBarContainer::BarPos MultiBarContainer::barPos() const
    {
        return d->barPos;
    }

    void MultiBarContainer::setBarPos( MultiBarContainer::BarPos position )
    {
        if( position != d->barPos )
        {
            d->barPos = position;
        }
    }

    int MultiBarContainer::addView( ViewContainerContent* view )
    {
        return insertView( d->views.count(), view );
    }

    int MultiBarContainer::insertView( int index, ViewContainerContent* view )
    {
        return -1;
    }

    void MultiBarContainer::takeView( int index )
    {
    }

}
