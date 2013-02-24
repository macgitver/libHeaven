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

#include <QDebug>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSplitter>

#include "libHeaven/Views/ViewContainer.h"
#include "libHeaven/Views/View.h"

#include "libHeaven/MultiBar/MultiBarContainer.hpp"

#include "Widgets/MiniSplitter.h"
#include "Widgets/TabWidget.h"

namespace Heaven
{

    ViewContainer::ViewContainer( const QString& identifier, Type t, Subtype s, ViewContainer* parent )
        : QObject( parent )
        , ViewContainerContent( identifier )
        , mType( t )
        , mSubtype( s )
        , mContainerWidget( NULL )
    {
        switch( t )
        {
        case Tab:       mContainerWidget = new TabWidget;               break;
        case MultiBar:  mContainerWidget = new MultiBarContainer;       break;
        case Splitter:  mContainerWidget = new MiniSplitter;            break;
        default:        Q_ASSERT( false );                              break;
        }
        setSubtype( s );
    }

    ViewContainer::~ViewContainer()
    {
    }

    ViewContainer::Type ViewContainer::type() const
    {
        return mType;
    }

    ViewContainer::Subtype ViewContainer::subtype() const
    {
        return mSubtype;
    }

    void ViewContainer::changeSubtype( Subtype subtype )
    {
        if( mSubtype != subtype )
        {
            setSubtype( subtype );
        }
    }

    void ViewContainer::setSubtype( Subtype subtype )
    {
        mSubtype = subtype;

        switch( mType )
        {
        case Tab:
            switch( subtype )
            {
            case SubTabLeft:    mTabWidget->setTabPos( TabBar::West );  break;
            case SubTabRight:   mTabWidget->setTabPos( TabBar::East );  break;
            case SubTabTop:     mTabWidget->setTabPos( TabBar::North ); break;
            case SubTabBottom:  mTabWidget->setTabPos( TabBar::South ); break;
            default: break;
            }

            break;

        case MultiBar:
            switch( subtype )
            {
            case SubTabLeft:    mMultiBarContainer->setBarPos( MultiBarContainer::West );   break;
            case SubTabRight:   mMultiBarContainer->setBarPos( MultiBarContainer::East );   break;
            case SubTabTop:     mMultiBarContainer->setBarPos( MultiBarContainer::North );  break;
            case SubTabBottom:  mMultiBarContainer->setBarPos( MultiBarContainer::South );  break;
            default: break;
            }

            break;

        case Splitter:
            mSpliterWidget->setOrientation( (subtype == SubSplitHorz ) ? Qt::Horizontal
                                                                       : Qt::Vertical );
            break;

        default:
            Q_ASSERT( false );
            break;
        }
    }

    void ViewContainer::clear()
    {
        while( mContents.count() )
        {
            if( mContents[ 0 ]->isContainer() )
            {
                ViewContainer* child = mContents[ 0 ]->asContainer();
                takeAt( 0 );
                child->clear();
                child->deleteLater();
            }
            else
            {
                View* child = mContents[ 0 ]->asView();
                takeAt( 0 );
                child->deleteLater();
            }
        }
    }

    void ViewContainer::add( ViewContainerContent* content )
    {
        if( content->isContainer() )
        {
            insertContainer( mContents.count(), content->asContainer() );
        }
        else
        {
            View* view = content->asView();
            if( view->container() )
            {
                view->container()->take( view );
            }
            mContents.append( view );
            view->setContainer( this );

            switch( mType )
            {
            case Tab:
                mTabWidget->addTab( view, view->viewName() );
                break;

            case MultiBar:
                mMultiBarContainer->addView( view );
                break;

            case Splitter:
                mSpliterWidget->addWidget( view );
                break;

            default:
                Q_ASSERT( false );
            }
        }
    }

    void ViewContainer::insertContainer( int pos, ViewContainer* container )
    {
        mContents.insert( pos, container );

        switch( mType )
        {
        case Tab:
            qDebug() << "libHeaven: Inserting container into another container...";
            mTabWidget->insertTab( pos, container->containerWidget(), trUtf8( "Container" ) );
            return;

        case MultiBar:
            qDebug() << "libHeaven: Inserting container into another container...";
            mMultiBarContainer->insertView( pos, container );
            break;

        case Splitter:
            mSpliterWidget->insertWidget( pos, container->containerWidget() );
            return;

        default:
            Q_ASSERT( false );
            return;
        }
    }

    int ViewContainer::indexOf( ViewContainerContent* cc ) const
    {
        return mContents.indexOf( cc );
    }

    ViewContainerContent* ViewContainer::take( ViewContainerContent* cc )
    {
        if( !cc )
        {
            return NULL;
        }

        Q_ASSERT( !cc->isContainer() );

        int i = indexOf( cc );
        Q_ASSERT( i != -1 );

        ViewContainerContent* cc2 = takeAt( i );
        Q_ASSERT( cc2 == cc );
        return cc2;
    }

    ViewContainerContent* ViewContainer::takeAt( int index )
    {
        ViewContainerContent* cc = mContents[ index ];
        mContents.removeAt( index );

        if( !cc )
        {
            return NULL;
        }

        cc->setContainer( NULL );
        QWidget* w = cc->widget();

        switch( mType )
        {
        case Tab:
            w->hide();
            w->setParent( NULL );
            mTabWidget->removeTab( index );
            break;

        case MultiBar:
            mMultiBarContainer->takeView( index );
            break;

        case Splitter:
            if( mSpliterWidget->indexOf( w ) == -1 )
            {
                w = w->parentWidget();
                Q_ASSERT( mSpliterWidget->indexOf( w ) != -1 );
                w->hide();
                w->setParent( NULL );
                w->deleteLater();
            }
            else
            {
                w->hide();
                w->setParent( NULL );
            }
            break;

        default:
            Q_ASSERT( false );
            return NULL;
        }

        return cc;
    }

    QWidget* ViewContainer::containerWidget()
    {
        return mContainerWidget;
    }

    bool ViewContainer::isContainer() const
    {
        return true;
    }

    ViewContainer* ViewContainer::asContainer()
    {
        return this;
    }

    QList< ViewContainerContent* > ViewContainer::contents() const
    {
        return mContents;
    }

    QWidget* ViewContainer::widget()
    {
        return mContainerWidget;
    }

}
