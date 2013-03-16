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

#ifndef MGV_HEAVEN_MODE_H
#define MGV_HEAVEN_MODE_H

#include <QObject>
#include <QSharedData>

#include "libHeaven/HeavenApi.hpp"

class QDomElement;

namespace Heaven
{

    class WindowStateRoot;
    class ModePrivate;
    class ApplicationPrivate;

    class HEAVEN_API Mode : public QObject
    {
        friend class ApplicationPrivate;
        Q_OBJECT
    public:
        Mode( const QString& name, const QDomElement& elParent );
        ~Mode();

    public:
        QString name() const;
        void setName( const QString& name );

        QString localConfigFile() const;

    public:
        void activate();
        void deactivate();

        void saveConfig();

    public:
        void reset();

    private:
        bool tryLoadConfig();
        QExplicitlySharedDataPointer< WindowStateRoot > state() const;

    signals:
        void nameChanged( const QString& name );
        void modeReset();

    private:
        ModePrivate* d;
    };

}

#endif
