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

#include <QPainter>
#include <QPaintEvent>

#include "Widgets/MultiBarViewWidget.hpp"
#include "Views/View.h"

namespace Heaven
{

    MultiBarViewWidget::MultiBarViewWidget( View* view, QWidget* parent )
        : QWidget( parent )
    {
        Q_ASSERT( view );
        mView = view;
        mText = view->viewName();
        mIsActive = false;
    }

    void MultiBarViewWidget::setActive( bool active )
    {
        if( active != mIsActive )
        {
            mIsActive = active;
            update();
        }
    }

    bool MultiBarViewWidget::isActive() const
    {
        return mIsActive;
    }

    View* MultiBarViewWidget::view() const
    {
        return mView;
    }

    QString MultiBarViewWidget::text() const
    {
        return mText;
    }

    void MultiBarViewWidget::paintEvent( QPaintEvent* ev )
    {
        QPainter p( this );
        p.fillRect( rect(), Qt::green );
    }

}
