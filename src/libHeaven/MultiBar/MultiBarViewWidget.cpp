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

#include <QPainter>
#include <QPaintEvent>

#include "Views/View.h"

#include "MultiBar/MultiBarViewWidget.hpp"

namespace Heaven
{

    MultiBarViewWidget::MultiBarViewWidget( View* view, QWidget* parent )
        : QWidget( parent )
    {
        Q_ASSERT( view );
        mView = view;
        mText = view->viewName();
        mIsPressed = mIsHovered = mIsActive = false;
        mOrientation = Qt::Horizontal;
    }

    void MultiBarViewWidget::setActive( bool active )
    {
        if( active != mIsActive )
        {
            mIsActive = active;
            mNiceSize = QSize();
            updateGeometry();
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

        if( mIsHovered || mIsActive )
        {
            QColor base( 0x40, 0x40, 0x40 );
            QLinearGradient grad1;

            if( mOrientation == Qt::Horizontal )
                grad1 = QLinearGradient( 0., 0., 0., 7. );
            else
                grad1 = QLinearGradient( 0., 0., 7., 0. );

            if( mIsPressed || mIsActive )
            {
                grad1.setColorAt( 0, base.darker( 80 ) );
                grad1.setColorAt( 0.8, base.darker( 120 ) );
                grad1.setColorAt( 1, base.darker( 120 ) );
            }
            else
            {
                grad1.setColorAt( 0, base.lighter( 180 ) );
                grad1.setColorAt( 0.8, base.lighter( 220 ) );
                grad1.setColorAt( 1, base.lighter( 220 ) );
            }
            QBrush b( grad1 );
            p.fillRect( rect(), b );
        }

        QFont f = font();
        if( mIsActive )
        {
            f.setBold( true );
        }
        p.setFont( f );

        QRect textRect = rect().adjusted( 4, 2, -4, -1 );
        p.setPen( Qt::white );
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

        QFont f( font() );
        f.setBold( true );

        QFontMetrics fm( f );
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

    void MultiBarViewWidget::mousePressEvent( QMouseEvent* ev )
    {
        if( ev->button() == Qt::LeftButton && mIsHovered )
        {
            mIsPressed = true;
            update();
        }
    }

    void MultiBarViewWidget::mouseReleaseEvent( QMouseEvent* ev )
    {
        if( ev->button() == Qt::LeftButton && mIsPressed )
        {
            mIsPressed = false;
            update();
            emit wantActivationChange( !mIsActive );
        }
    }

    void MultiBarViewWidget::leaveEvent( QEvent* )
    {
        if( mIsHovered || mIsPressed )
        {
            mIsPressed = false;
            mIsHovered = false;
            update();
        }
    }

    void MultiBarViewWidget::enterEvent( QEvent* )
    {
        mIsHovered = true;
        update();
    }

}
