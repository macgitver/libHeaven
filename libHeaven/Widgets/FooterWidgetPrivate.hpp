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

#ifndef MGV_HEAVEN_FOOTER_WIDGET_PRIVATE_H
#define MGV_HEAVEN_FOOTER_WIDGET_PRIVATE_H

#include <QList>

class QWidget;
class QSizeGrip;
class QBoxLayout;

namespace Heaven
{

    class FooterWidget;
    class FooterWidgetPrivate
    {
    public:
        struct Widget
        {
            QWidget*    widget;
            int         stretch;
            int         position;
        };
        typedef QList< Widget > WidgetList;

    public:
        FooterWidgetPrivate();

        void relayout();
        QWidget* takeWidget( QWidget* widget, int index );

    public:
        FooterWidget*   mOwner;
        int             mSpacing;
        int             mMarginH;
        int             mMarginV;
        WidgetList      mWidgets;
        QSizeGrip*      mGrip;
        QBoxLayout*     mLayout;
    };

}

#endif
