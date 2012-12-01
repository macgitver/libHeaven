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

#ifndef MGV_HEAVEN_APPLICATION_PRIVATE_H
#define MGV_HEAVEN_APPLICATION_PRIVATE_H

#include <QVector>

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

    public:
        static void setPrimaryWindow( PrimaryWindow* pw );

    public:
        Application*        owner;
        PrimaryWindow*      primaryWindow;
        Mode*               currentMode;
        QVector< Mode* >    modes;
    };

}

#endif


