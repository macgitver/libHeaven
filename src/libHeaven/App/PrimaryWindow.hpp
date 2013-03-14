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

#ifndef MGV_HEAVEN_PRIMARY_WINDOW_HPP
#define MGV_HEAVEN_PRIMARY_WINDOW_HPP

#include <QMainWindow>

#include "libHeaven/App/HeavenWindow.hpp"

namespace Heaven
{

    class MenuBar;
    class FooterWidget;

    class PrimaryWindowPrivate;

    class HEAVEN_API PrimaryWindow : public HeavenWindow
    {
        friend class PrimaryWindowPrivate;
        Q_OBJECT
    public:
        PrimaryWindow();
        ~PrimaryWindow();

    public:
        MenuBar* menuBar() const;
        void setMenuBar( MenuBar* bar );

        FooterWidget* statusBar() const;
        void setStatusBar( FooterWidget* bar );

    public:
        bool isPrimary() const;

    protected:
        bool event( QEvent* e );
    };

}

#endif
