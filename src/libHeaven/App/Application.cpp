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

#include "libHeaven/App/Application.hpp"
#include "libHeaven/App/ApplicationPrivate.hpp"

#include "libHeaven/Views/Mode.h"

namespace Heaven
{

    ApplicationPrivate::ApplicationPrivate()
    {
        primaryWindow = NULL;
    }

    Application* Application::sSelf = NULL;

    Application* Application::self()
    {
        if( !sSelf )
        {
            sSelf = new Application;
        }
        return sSelf;
    }

    Application::Application()
    {
        d = new ApplicationPrivate;
    }

    PrimaryWindow* Application::primaryWindow() const
    {
        return d->primaryWindow;
    }



    void Application::setCurrentMode( Mode* mode )
    {
    //  d->switchToMode( mode );
    }

    Mode* Application::findMode( const QString& name )
    {
        foreach( Mode* mode, d->mModes )
        {
            if( mode->name() == name )
            {
                return mode;
            }
        }

        return NULL;
    }

    void Application::addMode( Mode* mode )
    {
        Q_ASSERT( mode );
        mode->setParent( this );
        d->mModes.append( mode );
    }

    void Application::removeMode( Mode* mode )
    {
        if( currentMode() == mode )
        {
            // FIXME: Find a suitable mode
        //  d->switchToMode( NULL );
        }

        int i = d->mModes.indexOf( mode );
        Q_ASSERT( i != -1 );
        d->mModes.remove( i );
        delete mode;
    }

    Mode* Application::currentMode()
    {
        return d->mCurrentMode;
    }

}
