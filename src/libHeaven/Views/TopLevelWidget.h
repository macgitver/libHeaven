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

#ifndef HEAVEN_TOP_LEVEL_WIDGET_H
#define HEAVEN_TOP_LEVEL_WIDGET_H

#include <QWidget>

#include "libHeaven/HeavenApi.hpp"

#include "libHeaven/Views/ViewContainer.h"
#include "libHeaven/Views/View.h"

namespace Heaven
{

    class HEAVEN_API TopLevelWidget : public QWidget
    {
        Q_OBJECT
    public:
        TopLevelWidget( QWidget* parent = 0 );
        ~TopLevelWidget();

    public:
        void clear();

    public:
        ViewContainer* rootContainer();
        void setRootContainer( ViewContainer* c );

    public:
        void addContainer( ViewContainer* c );
        void addView( View* c, Positions pos = Left );

        QSet< View* > setOfViews() const;

    protected:
        void paintEvent( QPaintEvent* ev );

    private:
        ViewContainer*  mRoot;
    };

}

#endif
