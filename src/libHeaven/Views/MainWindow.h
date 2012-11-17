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

#ifndef MGV_HEAVEN_MAINWINDOW_H
#define MGV_HEAVEN_MAINWINDOW_H

#include <QMainWindow>

#include "libHeaven/HeavenApi.h"

namespace Heaven
{

    class Mode;
    class MenuBar;
    class TopLevelWidget;
    class FooterWidget;

    class MainWindowPrivate;

    /**
     * @brief       A Heaven-Application's primary window
     */
    class HEAVEN_API MainWindow : public QWidget
    {
        friend class MainWindowPrivate;
        Q_OBJECT
    public:
        MainWindow();
        ~MainWindow();

    public:
        void addMode( Mode* mode );
        void removeMode( Mode* mode );
        void setCurrentMode( Mode* mode );
        Mode* findMode( const QString& name );
        Mode* currentMode();

    public:
        MenuBar* menuBar();
        void setMenuBar( MenuBar* bar );

        FooterWidget* statusBar();
        void setStatusBar( FooterWidget* bar );

        // Temporary:
        TopLevelWidget* topLevelContainer();

    signals:
        void currentModeChanged( Heaven::Mode* newMode );

    protected:
        bool event( QEvent* e );

    private:
        MainWindowPrivate* d;
    };

}

#endif
