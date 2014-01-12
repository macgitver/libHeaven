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

#ifndef BLUESKY_VIEWS_HPP
#define BLUESKY_VIEWS_HPP
#pragma once

#include <QWidget>
#include <QPointer>

#include "libHeavenActions/ToolBar.hpp"

#include "libBlueSky/libBlueSkyAPI.hpp"

namespace BlueSky {

    class ContainerWidget;

    class HEAVEN_BLUESKY_API ViewIdentifier
    {
    public:
        ViewIdentifier();
        ViewIdentifier( const char* szName );
        ViewIdentifier( const ViewIdentifier& other );
        explicit ViewIdentifier( const QString& name );

    public:
        ViewIdentifier& operator=( const char* szName );
        ViewIdentifier& operator=( const ViewIdentifier& other );
        ViewIdentifier& operator=( const QString& name );

        bool operator==( const QString& other ) const;
        bool operator==( const ViewIdentifier& other ) const;
        bool operator!=( const QString& other ) const;
        bool operator!=( const ViewIdentifier& other ) const;

    public:
        QString toString() const;
        bool isValid() const;

    private:
        QString mName;
    };

    /**
     * @class   AbstractViewWidget
     * @ingroup CentralUI
     * @brief   Base class for all Central UI Widgets
     *
     *
     */
    class HEAVEN_BLUESKY_API AbstractViewWidget : public QWidget
    {
        Q_OBJECT
    public:
        AbstractViewWidget(const ViewIdentifier& identifier);

    public:
        virtual bool isContainerWidget() const;

    public:
        ViewIdentifier identifier() const;
        ContainerWidget* parentContainer();
        void setParentContainer(ContainerWidget* container);

    public:
        ContainerWidget* asContainerWidget();

    private:
        const ViewIdentifier    mIdentifier;        //!< Identifier of this View / ContainerWidget
        ContainerWidget*        mParentContainer;   //!< The parent container
    };



    class HEAVEN_BLUESKY_API View : public AbstractViewWidget
    {
        Q_OBJECT
        friend class MultiBarContainerWidget;
    public:
        View(const ViewIdentifier& identifier);
        ~View();

    public:
        QString viewName() const;
        void setViewName(const QString& name);

        void setToolBar(Heaven::ToolBar* tb);
        Heaven::ToolBar* toolBar() const;

        void setWidget(QWidget* widget);
        QWidget* widget();

    signals:
        void nameChanged(const QString& viewName);
        void toolBarChanged(Heaven::ToolBar* toolBar);

    public:
        virtual void closeView();

    protected:
        virtual void aboutToRemove();

    private:
        void queueRelayouting();
    private slots:
        void performQueuedRelayouting();

    private:
        QString                     mViewName;
        QPointer<Heaven::ToolBar>   mToolBar;
        QPointer<QWidget>           mWidget;
        bool                        mRelayoutingIsQueued    : 1;
        bool                        mRelayoutingForced      : 1;
        bool                        mToolBarInOwnLayout     : 1;
    };


    HEAVEN_BLUESKY_API uint qHash( const ViewIdentifier& id );
    HEAVEN_BLUESKY_API bool operator<(const ViewIdentifier& lhs, const ViewIdentifier& rhs);

}

HEAVEN_BLUESKY_API QDebug& operator<<(QDebug& stream, const BlueSky::ViewIdentifier& id);

#endif
