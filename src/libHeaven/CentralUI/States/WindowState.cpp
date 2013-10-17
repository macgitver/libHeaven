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
#include <QVariant>
#include <QIODevice>
#include <QByteArray>
#include <QUuid>
#include <QDomElement>
#include <QStringBuilder>

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

    void WindowState::readOptions(const QDomElement& el)
    {
        for (int i = 0; i < el.attributes().count(); i++) {
            QDomAttr a = el.attributes().item(i).toAttr();
            if (a.name().startsWith(QLatin1String("Opt"))) {
                QString opt = a.name().mid(4);
                QByteArray binData = a.value().toUtf8();
                QDataStream stream(&binData, QIODevice::ReadOnly);
                QVariant vData;
                stream >> vData;
                mOptions.insert(opt, vData);
            }
        }
    }

    void WindowState::saveOptions(QDomElement &el) const
    {
        foreach (QString name, mOptions.keys()) {
            QVariant vData = mOptions.value(name);
            QString sData;
            QByteArray binData;
            QDataStream stream(&binData, QIODevice::WriteOnly);
            stream << vData;
            sData = QString::fromUtf8(binData.constData());
            el.setAttribute(QLatin1Literal("Opt") % name, sData);
        }
    }

    void WindowState::setOption(const QString& name, const QVariant& vData)
    {
        mOptions[name] = vData;
    }

    void WindowState::unsetOption(const QString& name)
    {
        mOptions.remove(name);
    }

    QVariant WindowState::option(const QString& name) const
    {
        return mOptions.value(name, QVariant());
    }

    bool WindowState::isOptionSet(const QString& name) const
    {
        return mOptions.contains(name);
    }

    void WindowState::setWidget(QWidget* widget)
    {
        mWidget = widget;

        if (mWidget) {
            applyConfig();
        }
    }

    void WindowState::clearWidgets()
    {
        if (mWidget) {
            updateConfig();
        }

        mWidget = NULL;

        foreach (WindowState::Ptr pChild, mChildren) {
            pChild->clearWidgets();
        }
    }

    QWidget* WindowState::widget() const
    {
        return mWidget;
    }

}
