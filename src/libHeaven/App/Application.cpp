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
#include "libHeaven/App/PrimaryWindow.hpp"
#include "libHeaven/App/ModeSwitcher.hpp"
#include "libHeaven/App/Mode.hpp"

#include "libHeaven/CentralUI/ViewFactory.hpp"

static inline void initRes()
{
    Q_INIT_RESOURCE(Heaven);
}

namespace Heaven
{

    ApplicationPrivate::ApplicationPrivate()
    {
        owner = NULL;
        viewFactory = NULL;
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
            currentMode->disconnect( owner, SLOT(reapplyMode()) );
            currentMode->deactivate();
            currentMode = NULL;
        }

        currentMode = mode;

        if( currentMode )
        {
            applyMode();

            QObject::connect( currentMode, SIGNAL(modeReset()), owner, SLOT(reapplyMode()) );
            currentMode->activate();
        }

        emit owner->currentModeChanged( mode );
    }

    void ApplicationPrivate::applyMode()
    {
        ModeSwitcher ms( currentMode->state() );
        ms.run();
    }

    void ApplicationPrivate::setPrimaryWindow( PrimaryWindow* pw )
    {
        Application::self()->d->changePrimaryWindow( pw );
    }

    void ApplicationPrivate::changePrimaryWindow( PrimaryWindow* pw )
    {
        if( pw )
        {
            if( primaryWindow )
            {
                Q_ASSERT( heavenWindows.count() > 0 && heavenWindows.at( 0 ) == primaryWindow );
                heavenWindows[ 0 ] = pw;
            }
            else
            {
                Q_ASSERT( heavenWindows.count() == 0 );
                heavenWindows.append( pw );
            }
        }
        else
        {
            // TODO: Close all Secondaries
        }

        primaryWindow = pw;
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

    void Application::setCurrentMode( const QString& modeName )
    {
        setCurrentMode( findMode( modeName ) );
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

    /**
     * @brief       Change the view factory
     *
     * @param[in]   factory     The new ViewFactory to use for creating views.
     *
     * libHeaven takes ownership of the @a factory and deletes it as appropiate, which is usually
     * at the end of the application's life time or when you set a new ViewFacotry.
     *
     */
    void Application::setViewFactory( ViewFactory* factory )
    {
        if( d->viewFactory == factory )
        {
            return;
        }

        if( d->viewFactory )
        {
            d->viewFactory->deleteLater();
        }

        d->viewFactory = factory;

        if( d->viewFactory )
        {
            d->viewFactory->setParent( this );
        }
    }

    View* Application::createView( const QString& identifer )
    {
        if( d->viewFactory )
        {
            return d->viewFactory->createView( identifer );
        }

        return NULL;
    }

    SecondaryWindows Application::secondaryWindows() const
    {
        return d->secondaryWindows;
    }

    HeavenWindows Application::allWindows() const
    {
        return d->heavenWindows;
    }

    SecondaryWindow* Application::createSecondaryWindow()
    {
        SecondaryWindow* sw = new SecondaryWindow;
        d->secondaryWindows.append( sw );
        d->heavenWindows.append( sw );
        return sw;
    }

    HeavenWindow* Application::window( const QString& handle, bool create )
    {
        for( int i = 0; i < d->heavenWindows.count(); ++i )
        {
            if( handle == d->heavenWindows[ i ]->handle() )
            {
                return d->heavenWindows[ i ];
            }
        }

        HeavenWindow* hw = NULL;

        if( create )
        {
            hw = createSecondaryWindow();
            hw->setHandle( handle );
        }

        return hw;
    }

    void Application::reapplyMode()
    {
        d->applyMode();
    }

}
