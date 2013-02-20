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

#if QT_VERSION < 0x050000
    #include <QDesktopServices>
#else
    #include <QStandardPaths>
#endif

#include "libHeaven/App/Application.hpp"
#include "libHeaven/App/ApplicationPrivate.hpp"

#include "libHeaven/Views/Mode.h"

static inline void initRes()
{
    Q_INIT_RESOURCE(Heaven);
}

namespace Heaven
{

    ApplicationPrivate::ApplicationPrivate()
    {
        owner = NULL;
        currentMode = NULL;
        primaryWindow = NULL;
    }

    void ApplicationPrivate::switchToMode( Mode* mode )
    {
        if( mode == currentMode )
        {
            return;
        }

        if( currentMode )
        {
            currentMode->deactivate();
            currentMode = NULL;
        }

        if( mode )
        {
            mode->activate();
            currentMode = mode;
        }

        emit owner->currentModeChanged( mode );
    }

    void ApplicationPrivate::setPrimaryWindow( PrimaryWindow* pw )
    {
        Application::self()->d->primaryWindow = pw;
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
        d->owner = this;
        initRes();
    }

    PrimaryWindow* Application::primaryWindow() const
    {
        return d->primaryWindow;
    }

    void Application::setCurrentMode( Mode* mode )
    {
        d->switchToMode( mode );
    }

    Mode* Application::findMode( const QString& name )
    {
        foreach( Mode* mode, d->modes )
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
        d->modes.append( mode );
    }

    void Application::removeMode( Mode* mode )
    {
        if( currentMode() == mode )
        {
            // FIXME: Find a suitable mode
            d->switchToMode( NULL );
        }

        int i = d->modes.indexOf( mode );
        Q_ASSERT( i != -1 );
        d->modes.remove( i );
        delete mode;
    }

    Mode* Application::currentMode()
    {
        return d->currentMode;
    }

    QString Application::dataPath()
    {
        QString path;
        #if QT_VERSION < 0x050000
            path = QDesktopServices::storageLocation( QDesktopServices::DataLocation );
        #else
            path = QStandardPaths::writableLocation( QStandardPaths::DataLocation );
        #endif
        return path;
    }

}
