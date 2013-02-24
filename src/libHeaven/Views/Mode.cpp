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

#include <QDir>
#include <QFile>
#include <QStringBuilder>
#include <QDomDocument>
#include <QTextStream>

#include "Heaven.hpp"

#include "App/Application.hpp"

#include "Views/Mode.h"
#include "Views/ModePrivate.h"

namespace Heaven
{

    Mode::Mode( const QString& name, const QDomElement& elParent )
        : d( new ModePrivate )
    {
        d->mName = name;
        d->mOriginalState = new WindowStateRoot( elParent );

        if( !tryLoadConfig() )
        {
            d->mRoot = d->mOriginalState;
        }
    }

    Mode::~Mode()
    {
        delete d;
    }

    QString Mode::localConfigFile() const
    {
        QString name = d->mName;
        name.replace( QChar( L'#' ), QChar( L'_' ) );
        return Application::dataPath() % QChar( L'/' ) % name % QLatin1Literal( ".hmcfg" );
    }

    bool Mode::tryLoadConfig()
    {
        QString s = localConfigFile();
        if( !QFile::exists( s ) )
        {
            return false;
        }

        QFile file( s );
        if( !file.open( QFile::ReadOnly ) )
        {
            return false;
        }

        QDomDocument doc;
        if( !doc.setContent( &file ) )
        {
            return false;
        }

        QDomElement docEl = doc.documentElement();
        if( !docEl.isElement() || docEl.tagName() != QLatin1String( "HeavenModeConfig" ) )
        {
            return false;
        }

        d->mRoot = new WindowStateRoot( docEl );
        return true;
    }

    void Mode::saveConfig()
    {
        QDomDocument doc( QLatin1String( "HeavenModeConfig" ) );

        d->mRoot->updateConfig();

        QDomElement elRoot = doc.createElement( QLatin1String( "HeavenModeConfig" ) );
        doc.appendChild( elRoot );

        d->mRoot->save( elRoot );

        QString base = Application::dataPath();
        QDir().mkpath( base );

        QFile file( localConfigFile() );
        if( !file.open( QFile::WriteOnly ) )
        {
            return;
        }

        QTextStream ts( &file );

        doc.save( ts, 2 );
    }

    QString Mode::name() const
    {
        return d->mName;
    }

    void Mode::setName( const QString& name )
    {
        if( name != d->mName )
        {
            d->mName = name;
            emit nameChanged( name );
        }
    }

    void Mode::activate()
    {
        // Nothing to do here, right? Everything must be done in Application
    }

    void Mode::deactivate()
    {
        Q_ASSERT( d->mRoot );
        saveConfig();
    }

    WindowStateRoot::Ptr Mode::state() const
    {
        return d->mRoot;
    }

    void Mode::reset()
    {
        if( d->mRoot != d->mOriginalState )
        {
            d->mRoot = d->mOriginalState;
            emit modeReset();
        }
    }

}
