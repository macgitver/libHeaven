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

#ifndef MGV_HEAVEN_ACTION_H
#define MGV_HEAVEN_ACTION_H

#include <QString>
#include <QObject>
#include <QAction>

class QKeySequence;

#include "libHeavenActions/UiObject.hpp"

namespace Heaven
{

    class ActionGroup;
    class IconRef;

    class HEAVEN_ACTIONS_API Action : public UiObject
    {
        Q_OBJECT
    public:
        Action( QObject* parent );

    public:
        QString text() const;
        QString statusTip() const;
        QString toolTip() const;
        IconRef iconRef() const;
        QKeySequence shortcut() const;
        Qt::ShortcutContext shortcutContext() const;
        QAction::MenuRole menuRole() const;
        bool isEnabled() const;
        bool isCheckable() const;
        bool isChecked() const;
        bool isVisible() const;
        ActionGroup* group() const;

    public slots:
        void setText( const QString& text );
        void setToolTip( const QString& text );
        void setStatusTip( const QString& text );
        void setStatusToolTip( const QString& text );
        void setEnabled( bool v );
        void setDisabled( bool v );
        void setCheckable( bool v );
        void setChecked( bool v );
        void setIconRef( const QString& text );
        void setIconRef( const IconRef& ref );
        void setShortcut ( const QString & shortcut );
        void setShortcutContext( Qt::ShortcutContext context );
        void setMenuRole( QAction::MenuRole role );
        void setVisible( bool visible );
        void setGroup(Heaven::ActionGroup *group);

    signals:
        void triggered();
        void toggled( bool checked );

    public:
        QAction* actionFor( QObject* parent );
    };

}

#endif
