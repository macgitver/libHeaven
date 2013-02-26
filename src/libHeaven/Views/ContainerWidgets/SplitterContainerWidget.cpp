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

#include <QVBoxLayout>

#include "libHeaven/Views/ContainerWidgets/SplitterContainerWidget.hpp"

namespace Heaven
{

    SplitterContainerWidget::SplitterContainerWidget( QWidget* parent )
        : ContainerWidget( parent )
    {
        mSplitter = new MiniSplitter;
        QVBoxLayout* l = new QVBoxLayout;
        l->setMargin( 0 );
        l->setSpacing( 0 );
        l->addWidget( mSplitter );
        setLayout( l );
    }

    ContainerTypes SplitterContainerWidget::containerType() const
    {
        return SplitterContainerType;
    }

    int SplitterContainerWidget::insert( int index, AbstractViewWidget* view )
    {
        mSplitter->insertWidget( index, view );
        return mSplitter->indexOf( view );
    }

    AbstractViewWidget* SplitterContainerWidget::takeAt( int index )
    {
        AbstractViewWidget* vw = qobject_cast< AbstractViewWidget* >( mSplitter->widget( index ) );
        if( vw )
        {
            vw->hide();
            vw->setParent( NULL );
        }
        return vw;
    }

    int SplitterContainerWidget::indexOf( AbstractViewWidget* widget ) const
    {
        return mSplitter->indexOf( widget );
    }

    int SplitterContainerWidget::count() const
    {
        return mSplitter->count();
    }

    void SplitterContainerWidget::setVertical( bool vert )
    {
        mSplitter->setOrientation( vert ? Qt::Vertical : Qt::Horizontal );
    }

    bool SplitterContainerWidget::isVertical() const
    {
        return mSplitter->orientation() == Qt::Vertical;
    }

}
