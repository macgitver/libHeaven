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

#ifndef MGV_HEAVEN_APPLICATION_PRIVATE_H
#define MGV_HEAVEN_APPLICATION_PRIVATE_H

#include <QVector>

#include "App/SecondaryWindow.hpp"

namespace Heaven
{

    class PrimaryWindow;
    class Application;
    class Mode;

    class ApplicationPrivate
    {
    public:
        ApplicationPrivate();

    public:
        void switchToMode( Mode* mode );
        void applyMode();

    public:
        static void setPrimaryWindow( PrimaryWindow* pw );

    private:
        void changePrimaryWindow( PrimaryWindow* pw );

    public:
        Application*        owner;
        PrimaryWindow*      primaryWindow;
        SecondaryWindows    secondaryWindows;
        HeavenWindows       heavenWindows;
        Mode*               currentMode;
        QVector< Mode* >    modes;
    };

}

#endif


