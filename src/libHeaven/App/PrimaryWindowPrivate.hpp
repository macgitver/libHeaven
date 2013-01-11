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

#ifndef MGV_HEAVEN_PRIMARY_WINDOW_PRIVATE_HPP
#define MGV_HEAVEN_PRIMARY_WINDOW_PRIVATE_HPP

#include <QVector>
#include <QLayout>

class QStatusBar;
class QMenuBar;

namespace Heaven
{

    class PrimaryWindow;
    class PrimaryWindowPrivate;
    class Mode;
    class MenuBar;
    class ModeSwitchWidget;
    class TopLevelWidget;
    class FooterWidget;

    class PrimaryWindowLayout : public QLayout
    {
        Q_OBJECT
    public:
        PrimaryWindowLayout( PrimaryWindowPrivate* owner );

    public:
        void addItem( QLayoutItem* item );
        int count() const;
        QLayoutItem* itemAt( int index ) const;
        QLayoutItem* takeAt( int index );

        QSize sizeHint() const;
        QSize maximumSize() const;
        QSize minimumSize() const;

        void setGeometry( const QRect& rect );

        void setStatusBar( FooterWidget* bar );

    private:
        PrimaryWindowPrivate* mOwner;
        QWidgetItem* mTopLevel;
        QWidgetItem* mStatusBar;
    };

    class PrimaryWindowPrivate
    {
    public:
        PrimaryWindowPrivate();

    public:
        void setup();
        void switchToMode( Mode* mode );
        void updateLayouts();

    public:
        PrimaryWindow*          mOwner;
        PrimaryWindowLayout*    mLayout;
        QMenuBar*               mMenuBarWidget;
        FooterWidget*           mStatusBarWidget;
        MenuBar*                mMenuBar;
        ModeSwitchWidget*       mModeSwitchWidget;
        TopLevelWidget*         mTopLevelWidget;
    };

}

#endif
