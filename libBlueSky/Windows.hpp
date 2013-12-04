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

#ifndef BLUESKY_WINDOW_HPP
#define BLUESKY_WINDOW_HPP

#include <QWidget>

#include "libBlueSky/libBlueSkyAPI.hpp"

#include "libBlueSky/Views.hpp"

namespace Heaven {
    class MenuBar;
}

namespace BlueSky {

    class FooterWidget;

    class HEAVEN_BLUESKY_API Window : public QWidget {
        Q_OBJECT
    protected:
        class Data;
        Data* d;
        Window(Data *d);
    public:
        ~Window();

    public:
        static ViewIdentifier idPrimaryWindow();
        static ViewIdentifier idPrimarySplitter();

    public:
        ContainerWidget* rootContainer();
        ViewIdentifier identifier() const;
    };

    class HEAVEN_BLUESKY_API PrimaryWindow : public Window {
        Q_OBJECT
    private:
        class Data;

    public:
        PrimaryWindow();
        ~PrimaryWindow();

    public:
        void setMenuBar(Heaven::MenuBar* menu);
        Heaven::MenuBar* menuBar() const;

    public:
        FooterWidget* footer();

    private:
        Data* d();
        const Data* d() const;
    };

}

#endif
