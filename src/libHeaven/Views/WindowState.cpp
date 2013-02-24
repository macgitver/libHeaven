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
#include "libHeaven/Views/WindowState.hpp"

namespace Heaven
{

    static inline bool parseOrient( const QString& s )
    {
        return s == QLatin1String( "Vert" );
    }

    static inline ViewContainer::Subtype parseTabSubtype( const QString& s )
    {
        if( s == QLatin1String( "Left" ) )
            return ViewContainer::SubTabLeft;
        if( s == QLatin1String( "Right" ) )
            return ViewContainer::SubTabRight;
        if( s == QLatin1String( "Bottom" ) )
            return ViewContainer::SubTabBottom;

        return ViewContainer::SubTabTop;
    }

    WindowStateBase::WindowStateBase( WindowStateBase* parent )
        : mParent( parent )
        , mCurrentContent( NULL )
    {
        if( mParent )
        {
            mParent->mChildren.append( Ptr( this ) );
        }
    }

    WindowStateBase::~WindowStateBase()
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

    void WindowStateBase::readChildren( const QDomElement& elParent, ChildTypes allowed )
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

    void WindowStateBase::readOrCreateIdentifier( const QDomElement& el )
    {
        mId = el.attribute( QLatin1String( "Id" ), QString() );

        if( mId.isEmpty() )
        {
            mId = QUuid::createUuid().toString();
        }
    }

    void WindowStateBase::setCurrentContent( ViewContainerContent* vcc )
    {
        if( vcc != mCurrentContent )
        {
            if( mCurrentContent )
            {
                mCurrentContent->setWindowState( NULL );
            }

            mCurrentContent = vcc;

            if( mCurrentContent )
            {
                WindowStateBase* wsOld = mCurrentContent->windowState();
                if( wsOld )
                {
                    wsOld->setCurrentContent( NULL );
                }
                mCurrentContent->setWindowState( this );
            }
        }
    }

    void WindowStateBase::updateConfig()
    {
        foreach( const WindowStateBase::Ptr& ws, mChildren )
        {
            ws->updateConfig();
        }
    }

    ViewContainerContent* WindowStateBase::currentContent()
    {
        return mCurrentContent;
    }

    void WindowStateBase::saveIdentifier( QDomElement& el ) const
    {
        el.setAttribute( QLatin1String( "Id" ), mId );
    }

    void WindowStateBase::saveChildren( QDomElement& elParent ) const
    {
        foreach( const WindowStateBase::Ptr& ws, mChildren )
        {
            ws->save( elParent );
        }
    }

    int WindowStateBase::childrenCount() const
    {
        return mChildren.count();
    }

    WindowStateBase::Ptr WindowStateBase::childAt( int index ) const
    {
        return mChildren.at( index );
    }

    QVector< WindowStateBase::Ptr > WindowStateBase::children() const
    {
        return mChildren;
    }

    QString WindowStateBase::identifier() const
    {
        return mId;
    }

    void WindowStateBase::setIdentifier( const QString& id )
    {
        mId = id;
    }

    WindowStateSplitter::WindowStateSplitter( WindowStateBase* parent )
        : WindowStateBase( parent )
    {
    }

    WindowStateSplitter::WindowStateSplitter( WindowStateBase* parent, QDomElement& el )
        : WindowStateBase( parent )
    {
        mVertical = parseOrient( el.attribute( QLatin1String( "Orient" ),
                                               QLatin1String( "Vert" ) ) );

        readOrCreateIdentifier( el );
        readChildren( el, CTContainers | CTViews );
    }

    void WindowStateSplitter::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Split" ) );
        elParent.appendChild( elChild );

        elChild.setAttribute( QLatin1String( "Orient" ),
                              QLatin1String( mVertical ? "Vert" : "Horz" ) );

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

    void WindowStateSplitter::updateConfig()
    {
        WindowStateBase::updateConfig();
    }

    WindowStateBase::Type WindowStateSplitter::type() const
    {
        return WSSplitter;
    }

    void WindowStateSplitter::setVertical( bool value )
    {
        mVertical = value;
    }

    bool WindowStateSplitter::isVertical() const
    {
        return mVertical;
    }

    WindowStateTab::WindowStateTab( WindowStateBase* parent, QDomElement& el )
        : WindowStateBase( parent )
    {
        mTabSubType = parseTabSubtype( el.attribute( QLatin1String( "Pos" ),
                                                     QLatin1String( "Top" ) ) );

        readOrCreateIdentifier( el );
        readChildren( el, CTContainers | CTViews );
    }

    WindowStateTab::WindowStateTab( WindowStateBase* parent )
        : WindowStateBase( parent )
    {
    }

    void WindowStateTab::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Tab" ) );
        elParent.appendChild( elChild );

        const char* s = NULL;
        switch( mTabSubType )
        {
        case ViewContainer::SubTabBottom:   s = "Bottom";   break;
        case ViewContainer::SubTabTop:      s = "Top";      break;
        case ViewContainer::SubTabLeft:     s = "Left";     break;
        case ViewContainer::SubTabRight:    s = "Right";    break;
        default:                                            break;
        }

        if( s )
        {
            elChild.setAttribute( QLatin1String( "Pos" ), QLatin1String( s ) );
        }

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

    void WindowStateTab::updateConfig()
    {
        WindowStateBase::updateConfig();
    }

    WindowStateBase::Type WindowStateTab::type() const
    {
        return WSTab;
    }

    void WindowStateTab::setTabSubType( ViewContainer::Subtype subtype )
    {
        mTabSubType = subtype;
    }

    ViewContainer::Subtype WindowStateTab::subtype() const
    {
        return mTabSubType;
    }

    WindowStateView::WindowStateView( WindowStateBase* parent )
        : WindowStateBase( parent )
    {
    }

    WindowStateView::WindowStateView( WindowStateBase* parent, QDomElement& el )
        : WindowStateBase( parent )
    {
        readOrCreateIdentifier( el );
    }

    void WindowStateView::updateConfig()
    {
    }

    void WindowStateView::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "View" ) );
        saveIdentifier( elChild );
        elParent.appendChild( elChild );
    }

    WindowStateBase::Type WindowStateView::type() const
    {
        return WSView;
    }

    WindowStateWindow::WindowStateWindow( WindowStateBase* parent )
        : WindowStateBase( parent )
    {
    }

    WindowStateWindow::WindowStateWindow( WindowStateBase* parent, QDomElement& elParent )
        : WindowStateBase( parent )
    {
        readOrCreateIdentifier( elParent );
        readChildren( elParent, CTContainers );
    }

    WindowStateBase::Type WindowStateWindow::type() const
    {
        return WSWindow;
    }

    void WindowStateWindow::save( QDomElement& elParent ) const
    {
        QDomElement elChild = elParent.ownerDocument().createElement( QLatin1String( "Window" ) );
        elParent.appendChild( elChild );

        saveIdentifier( elChild );
        saveChildren( elChild );
    }

    WindowStateRoot::WindowStateRoot()
        : WindowStateBase( NULL )
    {
    }

    WindowStateRoot::WindowStateRoot( const QDomElement& elParent )
        : WindowStateBase( NULL )
    {
        load( elParent );
    }

    WindowStateRoot::~WindowStateRoot()
    {
    }

    WindowStateBase::Type WindowStateRoot::type() const
    {
        return WSRoot;
    }


    void WindowStateRoot::save( QDomElement& elParent ) const
    {
        for( int i = 0; i < childrenCount(); ++i )
        {
            WindowStateBase::Ptr ws = childAt( i );
            ws->save( elParent );
        }
    }

    void WindowStateRoot::load( const QDomElement& elParent )
    {
        readChildren( elParent, CTWindows );
    }

}
