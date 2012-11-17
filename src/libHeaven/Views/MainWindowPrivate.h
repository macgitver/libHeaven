/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_HEAVEN_MAINWINDOW_PRIVATE_H
#define MGV_HEAVEN_MAINWINDOW_PRIVATE_H

#include <QVector>
#include <QLayout>

class QStatusBar;
class QMenuBar;

namespace Heaven
{

    class MainWindow;
    class MainWindowPrivate;
    class Mode;
    class MenuBar;
    class ModeSwitchWidget;
    class TopLevelWidget;
    class FooterWidget;

    class MainWindowLayout : public QLayout
    {
        Q_OBJECT
    public:
        MainWindowLayout( MainWindowPrivate* owner );

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
        MainWindowPrivate* mOwner;
        QWidgetItem* mTopLevel;
        QWidgetItem* mStatusBar;
    };

    class MainWindowPrivate
    {
    public:
        MainWindowPrivate();

    public:
        void setup();
        void switchToMode( Mode* mode );
        void updateLayouts();

    public:
        MainWindow*         mOwner;
        MainWindowLayout*   mLayout;
        Mode*               mCurrentMode;
        QMenuBar*           mMenuBarWidget;
        FooterWidget*       mStatusBarWidget;
        MenuBar*            mMenuBar;
        ModeSwitchWidget*   mModeSwitchWidget;
        TopLevelWidget*     mTopLevelWidget;
        QVector< Mode* >    mModes;
    };

}

#endif
