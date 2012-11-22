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

#ifndef HEAVEN_MULTI_BAR_SECTION_HPP
#define HEAVEN_MULTI_BAR_SECTION_HPP

#include "libHeaven/HeavenApi.h"

#include <QWidget>

namespace Heaven
{

    class MultiBarSectionPrivate;

    class HEAVEN_API MultiBarSection : public QWidget
    {
        Q_OBJECT
    public:
        enum Flag
        {
            BreakAfter          = 1 << 0,
            NoSeparator         = 1 << 1,

            IsToolBar           = 1 << 16,
            IsViewsBar          = 1 << 17
        };
        typedef QFlags< Flag > Flags;

    protected:
        MultiBarSection( QWidget* parent, MultiBarSectionPrivate* p );

    public:
        MultiBarSection( QWidget* parent = 0 );
        ~MultiBarSection();

    public:
        void addWidget( QWidget* widget );
        void insertWidget( QWidget* widget, int index );
        void removeWidget( QWidget* widget );
        void removeWidget( int index );
        QWidget* takeWidget( QWidget* widget );
        QWidget* takeWidget( int index );
        int widgetCount() const;
        int indexOf( QWidget* widget ) const;
        QWidget* widgetAt( int index ) const;

        Flags flags() const;
        bool testFlag( Flag flag ) const;
        void setFlag( Flag flag, bool set = true );

    protected:
        MultiBarSectionPrivate* d;
    };

}

#endif
