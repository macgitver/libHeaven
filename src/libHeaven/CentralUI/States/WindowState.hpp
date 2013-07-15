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

#ifndef HEAVEN_WINDOW_STATE_HPP
#define HEAVEN_WINDOW_STATE_HPP

#include <QSharedData>
#include <QVector>
#include <QSet>

class QDomElement;

#include "libHeaven/Heaven.hpp"
#include "libHeaven/CentralUI/Views/AbstractViewWidget.hpp"

namespace Heaven
{

    class WindowState : public QSharedData
    {
    public:
        enum Type
        {
            WSSplitter = 0,
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
        typedef QExplicitlySharedDataPointer< WindowState > Ptr;

    public:
        WindowState( WindowState* parent );
        virtual ~WindowState();

    public:
        virtual Type type() const = 0;

        int childrenCount() const;
        WindowState::Ptr childAt( int index ) const;
        QVector< WindowState::Ptr > children() const;

        virtual void updateConfig();

    public:
        ViewIdentifier identifier() const;
        void setIdentifier( const ViewIdentifier& id );

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
        WindowState*                mParent;
        AbstractViewWidget  *           mCurrentContent;
        QVector< WindowState::Ptr > mChildren;
        ViewIdentifier                  mId;
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS( Heaven::WindowState::ChildTypes );

#endif
