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

#include "MultiBar/MultiBarViewSection.hpp"
#include "MultiBar/MultiBarSectionPrivate.hpp"
#include "MultiBar/MultiBarViewWidget.hpp"

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

        connect( w, SIGNAL(wantActivationChange(bool)),
                 this, SLOT(activationChange(bool)) );

        w->setOrientation( data->orientation );
        data->views.insert( index, w );

        if( data->activeView == -1 )
        {
            Q_ASSERT( index == 0 );
            data->activeView = 0;
            w->setActive( true );
        }
        else if( index < data->activeView )
        {
            data->activeView++;
        }

        insertWidget( w, index );
    }

    int MultiBarViewSection::indexOfView( View* view ) const
    {
        const MultiBarViewSectionPrivate* data = static_cast< const MultiBarViewSectionPrivate* >( d );
        for( int i = 0; i < data->views.count(); ++i )
        {
            if( data->views.at( i )->view() == view )
            {
                return i;
            }
        }

        return -1;
    }

    void MultiBarViewSection::removeView( View* view )
    {
        MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );
        int idx = indexOfView( view );
        int nextActive = -2;

        if( !view || idx == -1 )
        {
            return;
        }

        if( data->activeView == idx )
        {
            if( idx != 0 || data->views.count() == 1 )
            {
                nextActive = idx - 1;
            }
            else
            {
                nextActive = idx;
            }
            data->views.at( idx )->setActive( false );
            data->activeView = -1;
        }

        removeWidget( idx );
        data->views.removeAt( idx );

        if( nextActive != -2 )
        {
            setActiveView( nextActive );
            emit currentChanged( data->activeView );
        }
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


    void MultiBarViewSection::activationChange( bool desiredActivation )
    {
        MultiBarViewSectionPrivate* data = static_cast< MultiBarViewSectionPrivate* >( d );

        MultiBarViewWidget* w = qobject_cast< MultiBarViewWidget* >( sender() );
        int oldActive = data->activeView;

        if( !w )
        {
            return;
        }

        if( !desiredActivation )
        {
            return;
        }

        for( int i = 0; i < data->views.count(); i++ )
        {
            MultiBarViewWidget* dest = data->views.at( i );
            if( i == oldActive )
            {
                dest->setActive( false );
            }
            if( dest == w )
            {
                w->setActive( true );
                data->activeView = i;
            }
        }

        emit currentChanged( data->activeView );
    }

}
