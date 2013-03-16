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

#include <QDebug>
#include <QUuid>
#include <QDomElement>

#include "libHeaven/App/Application.hpp"
#include "libHeaven/App/PrimaryWindow.hpp"

#include "libHeaven/CentralUI/States/WindowState.hpp"
#include "libHeaven/CentralUI/States/WindowStateSplitter.hpp"
#include "libHeaven/CentralUI/States/WindowStateTab.hpp"
#include "libHeaven/CentralUI/States/WindowStateWindow.hpp"
#include "libHeaven/CentralUI/States/WindowStateView.hpp"

namespace Heaven
{

    WindowState::WindowState( WindowState* parent )
        : mParent( parent )
        , mCurrentContent( NULL )
    {
        if( mParent )
        {
            mParent->mChildren.append( Ptr( this ) );
        }
    }

    WindowState::~WindowState()
    {
        Q_ASSERT( mChildren.count() == 0 );

        if( mParent )
        {
            int i = mParent->mChildren.indexOf( Ptr( this ) );
            if( i != -1 )
            {
                mParent->mChildren.remove( i );
            }
        }
    }

    void WindowState::readChildren( const QDomElement& elParent, ChildTypes allowed )
    {
        QDomElement e = elParent.firstChildElement();
        while( e.isElement() )
        {
            QString tag = e.tagName();

            if( tag == QLatin1String( "Split" ) && ( allowed & CTContainers ) )
            {
                new WindowStateSplitter( this, e );
            }
            else if( tag == QLatin1String( "Tab" ) && ( allowed & CTContainers ) )
            {
                new WindowStateTab( this, e );
            }
            else if( tag == QLatin1String( "View" ) && ( allowed & CTViews ) )
            {
                new WindowStateView( this, e );
            }
            else if( tag == QLatin1String( "Window" ) && ( allowed & CTWindows ) )
            {
                new WindowStateWindow( this, e );
            }
            else
            {
                qDebug() << "Ignoring invalid Window State tag: " << tag;
            }

            e = e.nextSiblingElement();
        }
    }

    void WindowState::readOrCreateIdentifier( const QDomElement& el )
    {
        mId = el.attribute( QLatin1String( "Id" ), QString() );

        if( !mId.isValid() )
        {
            mId = QUuid::createUuid().toString();
        }
    }

    void WindowState::setCurrentContent( AbstractViewWidget* avw )
    {
        if( avw != mCurrentContent )
        {
            if( mCurrentContent )
            {
                // TODO: mCurrentContent->setWindowState( NULL );
            }

            mCurrentContent = avw;

            if( mCurrentContent )
            {
                /* TODO:
                WindowStateBase* wsOld = mCurrentContent->windowState();
                if( wsOld )
                {
                    wsOld->setCurrentContent( NULL );
                }
                mCurrentContent->setWindowState( this );
                */
            }
        }
    }

    void WindowState::updateConfig()
    {
        foreach( const WindowState::Ptr& ws, mChildren )
        {
            ws->updateConfig();
        }
    }

    AbstractViewWidget* WindowState::currentContent()
    {
        return mCurrentContent;
    }

    void WindowState::saveIdentifier( QDomElement& el ) const
    {
        el.setAttribute( QLatin1String( "Id" ), mId.toString() );
    }

    void WindowState::saveChildren( QDomElement& elParent ) const
    {
        foreach( const WindowState::Ptr& ws, mChildren )
        {
            ws->save( elParent );
        }
    }

    int WindowState::childrenCount() const
    {
        return mChildren.count();
    }

    WindowState::Ptr WindowState::childAt( int index ) const
    {
        return mChildren.at( index );
    }

    QVector< WindowState::Ptr > WindowState::children() const
    {
        return mChildren;
    }

    ViewIdentifier WindowState::identifier() const
    {
        return mId;
    }

    void WindowState::setIdentifier( const ViewIdentifier& id )
    {
        mId = id;
    }

}
