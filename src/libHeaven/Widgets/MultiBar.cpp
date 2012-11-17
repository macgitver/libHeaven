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

#include <QBoxLayout>

#include "Widgets/MultiBar.hpp"
#include "Widgets/MultiBarSection.hpp"
#include "Widgets/MultiBarPrivate.hpp"

namespace Heaven
{

    MultiBarPrivate::MultiBarPrivate()
        : owner( NULL )
        , layout( NULL )
    {
    }

    void MultiBarPrivate::relayout()
    {
        delete layout;

        layout = new QHBoxLayout( owner );
    }

    MultiBar::MultiBar( QWidget* parent )
        : QWidget( parent )
    {
        d = new MultiBarPrivate;
        d->owner = this;
        d->relayout();
    }

    MultiBar::~MultiBar()
    {
        delete d;
    }

    int MultiBar::sectionCount() const
    {
        return d->sections.count();
    }

    int MultiBar::addSection()
    {
        d->sections.append( new MultiBarSection( this ) );
        d->relayout();
        return d->sections.count() - 1;
    }

    void MultiBar::removeSection( int index )
    {
        d->sections.removeAt( index );
        d->relayout();
    }

    void MultiBar::removeAllSections()
    {
        while( d->sections.count() )
        {
            removeSection( 0 );
        }
    }

}
