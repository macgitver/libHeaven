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
#include "Widgets/MultiBarSectionPrivate.hpp"

namespace Heaven
{

    MultiBarSectionPrivate::MultiBarSectionPrivate()
    {
        owner = NULL;
        layout = NULL;
    }

    void MultiBarSectionPrivate::relayout()
    {
        delete layout;

        layout = new QHBoxLayout( owner );

        layout->addSpacing( 2 );

        for( int i = 0; i < widgets.count(); ++i )
        {
            QWidget* widget = widgets.at( i );
            layout->addWidget( widget );
        }

        layout->addSpacing( 2 );
    }

    MultiBarSection::MultiBarSection( QWidget* parent )
        : QWidget( parent )
    {
        d = new MultiBarSectionPrivate;
        d->owner = this;

        d->relayout();
    }

    MultiBarSection::MultiBarSection( QWidget* parent, MultiBarSectionPrivate* p )
        : QWidget( parent )
        , d( p )
    {
        d->owner = this;

        d->relayout();
    }

    MultiBarSection::~MultiBarSection()
    {
        delete d;
    }

    void MultiBarSection::addWidget( QWidget* widget )
    {
        insertWidget( widget, d->widgets.count() );
    }

    void MultiBarSection::insertWidget( QWidget* widget, int index )
    {
        d->widgets.insert( index, widget );
        d->relayout();
    }

    void MultiBarSection::removeWidget( QWidget* widget )
    {
        delete takeWidget( widget );
    }

    void MultiBarSection::removeWidget( int index )
    {
        delete takeWidget( index );
    }

    QWidget* MultiBarSection::takeWidget( QWidget* widget )
    {
        d->widgets.removeOne( widget );

        widget->hide();
        widget->setParent( NULL );
        d->relayout();

        return widget;
    }

    QWidget* MultiBarSection::takeWidget( int index )
    {
        return takeWidget( d->widgets.at( index ) );
    }

    int MultiBarSection::widgetCount() const
    {
        return d->widgets.count();
    }

    int MultiBarSection::indexOf( QWidget* widget ) const
    {
        return d->widgets.indexOf( widget );
    }

    QWidget* MultiBarSection::widgetAt( int index ) const
    {
        return d->widgets.at( index );
    }

    MultiBarSection::Flags MultiBarSection::flags() const
    {
        return d->flags;
    }

    bool MultiBarSection::testFlag( MultiBarSection::Flag flag ) const
    {
        return d->flags.testFlag( flag );
    }

    void MultiBarSection::setFlag( Flag flag, bool set )
    {
        if( set )
            d->flags |= flag;
        else
            d->flags &= ~flag;
    }

}
