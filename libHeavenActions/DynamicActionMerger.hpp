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

#ifndef MGV_HEAVEN_DYNAMIC_ACTION_MERGER_HPP
#define MGV_HEAVEN_DYNAMIC_ACTION_MERGER_HPP

#include <QString>
#include <QVariant>
#include <QObject>

#include "libHeavenActions/UiObject.hpp"

class QAction;

namespace Heaven
{

    enum MergerMode
    {
        DAMergerCallback,
        DAMergerAdvancedList
    };

    enum MergerActionLifetime
    {
        DAMergerActionUserControlled,
        DAMergerActionMergerControlled
    };

    class DynamicActionMergerPrivate;

    class HEAVEN_ACTIONS_API DynamicActionMerger : public UiObject
    {
        friend class DynamicActionMergerPrivate;
        Q_OBJECT
    public:
        DynamicActionMerger( QObject* parent );

    public:
        void setMode( MergerMode mode );
        MergerMode mode() const;

    signals:
        void entryTriggered( const QVariant& value );

    public:
        void clear();
        void addStringList( const QStringList& list );
        void addSeparator();
        void addAction( const QString& display, const QVariant& value );
        void addAction( QAction* act, const QVariant& value = QVariant(),
                        MergerActionLifetime lifeTime = DAMergerActionUserControlled );

    public slots:
        void triggerRebuild();

    public slots:
        void setMergerSlot( const char* szSlot );
    };

}

#endif
