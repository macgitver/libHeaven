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

#ifndef MGV_HEAVEN_FOOTER_WIDGET_H
#define MGV_HEAVEN_FOOTER_WIDGET_H

#include "libHeaven/HeavenApi.hpp"

#include <QWidget>

namespace Heaven
{

    class FooterWidgetPrivate;

    class HEAVEN_API FooterWidget : public QWidget
    {
        Q_OBJECT
    public:
        FooterWidget( QWidget* parent = 0 );
        ~FooterWidget();

    public:
        void setMargins( int horizontal, int vertical );
        void setSpacing( int spacing );

        int horizontalMargin() const;
        int verticalMargin() const;
        int spacing() const;

        void clear();

        void addWidget( QWidget* widget, Qt::Alignment align = Qt::AlignRight, int stretch = 0 );
        int widgetCount() const;
        QWidget* widgetAt( int index ) const;
        int indexOf( QWidget* widget ) const;

        void removeWidget( QWidget* widget );
        void removeWidget( int index );

        QWidget* takeWidget( int index );
        QWidget* takeWidget( QWidget* widget );

    protected:
        bool event( QEvent* ev );
        void paintEvent( QPaintEvent* );

    private:
        FooterWidgetPrivate* d;
    };

}

#endif
