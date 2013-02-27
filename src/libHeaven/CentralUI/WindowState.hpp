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

#ifndef HEAVEN_WINDOW_STATE_H
#define HEAVEN_WINDOW_STATE_H

#include <QSharedData>
#include <QVector>
#include <QSet>

class QDomElement;

#include "libHeaven/HeavenApi.hpp"
#include "libHeaven/Heaven.hpp"
#include "libHeaven/CentralUI/Views/AbstractViewWidget.hpp"

namespace Heaven
{

    class HEAVEN_API WindowStateBase : public QSharedData
    {
    public:
        enum Type
        {
            WSSplitter,
            WSTab,
            WSView,
            WSWindow,

            WSRoot
        };

        enum ChildType
        {
            CTContainers = 1 << 0,
            CTViews = 1 << 1,
            CTWindows = 1 << 2
        };
        Q_DECLARE_FLAGS( ChildTypes, ChildType )

    public:
        typedef QExplicitlySharedDataPointer< WindowStateBase > Ptr;

    public:
        WindowStateBase( WindowStateBase* parent );
        virtual ~WindowStateBase();

    public:
        virtual Type type() const = 0;

        int childrenCount() const;
        WindowStateBase::Ptr childAt( int index ) const;
        QVector< WindowStateBase::Ptr > children() const;

        virtual void updateConfig();

    public:
        QString identifier() const;
        void setIdentifier( const QString& id );

        void setCurrentContent( AbstractViewWidget* avw );
        AbstractViewWidget* currentContent();

    protected:
        void readChildren( const QDomElement& elParent, ChildTypes allowed );
        void readOrCreateIdentifier( const QDomElement& el );

        void saveChildren( QDomElement& elParent ) const;
        void saveIdentifier( QDomElement& el ) const;

    public:
        virtual void save( QDomElement& elParent ) const = 0;

    private:
        WindowStateBase*                mParent;
        AbstractViewWidget  *           mCurrentContent;
        QVector< WindowStateBase::Ptr > mChildren;
        QString                         mId;
    };

    class HEAVEN_API WindowStateSplitter : public WindowStateBase
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateSplitter > Ptr;

    public:
        WindowStateSplitter( WindowStateBase* parent );
        WindowStateSplitter( WindowStateBase* parent, QDomElement& el );

    public:
        Type type() const;

        void setVertical( bool value );
        bool isVertical() const;

        void updateConfig();

    protected:
        void save( QDomElement& elParent ) const;

    private:
        bool            mVertical;
        QVector< int >  mWidths;
    };

    class HEAVEN_API WindowStateTab : public WindowStateBase
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateTab > Ptr;

    public:
        WindowStateTab( WindowStateBase* parent );
        WindowStateTab( WindowStateBase* parent, QDomElement& el );

    public:
        Type type() const;

        void setTabPosition( Positions pos );
        Positions tabPosition() const;

        void updateConfig();

    protected:
        void save( QDomElement& elParent ) const;

    private:
        Positions mPositions;
    };

    class HEAVEN_API WindowStateView : public WindowStateBase
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateView > Ptr;

    public:
        WindowStateView( WindowStateBase* parent );
        WindowStateView( WindowStateBase* parent, QDomElement& el );

    public:
        Type type() const;

        void updateConfig();

    protected:
        void save( QDomElement& elParent ) const;
    };

    class HEAVEN_API WindowStateWindow : public WindowStateBase
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateWindow > Ptr;

    public:
        WindowStateWindow( WindowStateBase* parent );
        WindowStateWindow( WindowStateBase* parent, QDomElement& el );

    public:
        Type type() const;

    protected:
        void save( QDomElement& elParent ) const;
    };

    class HEAVEN_API WindowStateRoot : public WindowStateBase
    {
    public:
        typedef QExplicitlySharedDataPointer< WindowStateRoot > Ptr;

    public:
        WindowStateRoot();
        WindowStateRoot( const QDomElement& elParent );

        ~WindowStateRoot();

    public:
        Type type() const;

        void save( QDomElement& elParent ) const;

    private:
        void load( const QDomElement& elParent );
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS( Heaven::WindowStateBase::ChildTypes );

#endif
