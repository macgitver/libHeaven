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
        mOrientation = Qt::Horizontal;
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

    void MultiBarViewWidget::setOrientation( Qt::Orientation orientation )
    {
        if( orientation != mOrientation )
        {
            mOrientation = orientation;
            mNiceSize = QSize();

            updateGeometry();
            update();
        }
    }

    Qt::Orientation MultiBarViewWidget::orientation() const
    {
        return mOrientation;
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
        p.drawRect( rect().adjusted( 0, 0, -1, -1 ) );


        QRect textRect = rect().adjusted( 4, 2, -4, -1 );

        if( mOrientation == Qt::Horizontal )
        {
            p.drawText( textRect, Qt::AlignCenter, mText );
        }
        else
        {
            p.save();
            p.translate( textRect.bottomLeft() );
            p.rotate( 270 );
            QRect r( 0, 0, textRect.height(), textRect.width() );
            p.drawText( r, Qt::AlignCenter, mText );
            p.restore();
        }
    }

    void MultiBarViewWidget::calcNiceSize() const
    {
        if( mNiceSize.isValid() )
        {
            return;
        }

        QFontMetrics fm( font() );
        int w = 8 + fm.width( mText );
        int h = 4 + fm.lineSpacing();

        mNiceSize = ( mOrientation == Qt::Horizontal )
                ? QSize( w, h )
                : QSize( h, w );
    }

    QSize MultiBarViewWidget::minimumSizeHint() const
    {
        calcNiceSize();
        return mNiceSize;
    }

    QSize MultiBarViewWidget::sizeHint() const
    {
        calcNiceSize();
        return mNiceSize;
    }

}
