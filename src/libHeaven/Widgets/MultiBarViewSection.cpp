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

#include "Widgets/MultiBarViewSection.hpp"
#include "Widgets/MultiBarSectionPrivate.hpp"
#include "Widgets/MultiBarViewWidget.hpp"

namespace Heaven
{

    class MultiBarViewSectionPrivate : public MultiBarSectionPrivate
    {
    public:
        MultiBarViewSectionPrivate();

    public:
        int                             activeView;
        QList< MultiBarViewWidget* >    views;
    };

    MultiBarViewSectionPrivate::MultiBarViewSectionPrivate()
    {
        activeView = -1;
    }

    MultiBarViewSection::MultiBarViewSection( QWidget* parent )
        : MultiBarSection( parent, new MultiBarViewSectionPrivate )
    {
        setFlag( IsViewsBar );
    }

    void MultiBarViewSection::insertView( int index, View* view )
    {
        MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

        MultiBarViewWidget* w = new MultiBarViewWidget( view );
        w->setOrientation( data->orientation );
        data->views.insert( index, w );

        if( index > data->activeView )
        {
            data->activeView++;
        }

        insertWidget( w, index );
    }

    void MultiBarViewSection::removeView( View* view )
    {
    }

    void MultiBarViewSection::setActiveView( int index )
    {
        MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

        if( data->activeView != index )
        {
            if( data->activeView != -1 )
            {
                data->views[ data->activeView ]->setActive( false );
            }

            data->activeView = index;

            if( data->activeView != -1 )
            {
                data->views[ data->activeView ]->setActive( true );
            }
        }
    }

    int MultiBarViewSection::activeView() const
    {
        const MultiBarViewSectionPrivate* data = static_cast< const MultiBarViewSectionPrivate* >( d );
        return data->activeView;
    }

    void MultiBarViewSection::setOrientation( Qt::Orientation orientation )
    {
        MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

        foreach( MultiBarViewWidget* w, data->views )
        {
            w->setOrientation( orientation );
        }

        MultiBarSection::setOrientation( orientation );
    }

}
