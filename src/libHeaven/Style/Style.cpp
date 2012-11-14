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
#include <QMenuBar>
#include <QStyleOption>

#if QT_VERSION >= 0x050000
#include <qdrawutil.h>
#endif

#include "Style/Style.h"

namespace Heaven
{

    static bool isStyled( const QWidget* w )
    {
        if( !w ||
            ( w->window()->windowFlags() & Qt::WindowType_Mask ) == Qt::Dialog )
        {
            return false;
        }

        for( const QWidget* p = w; p; p = p->parentWidget() )
        {
            if( qobject_cast< const QMenuBar* >( p ) ||
                p->property( "heavenStyle" ).toBool() )
            {
                return true;
            }
        }

        return false;
    }

    Style::Style( QStyle* baseStyle )
        : QProxyStyle( baseStyle )
    {
        QColor base( QColor( 0x40, 0x40, 0x40 ) );
        QLinearGradient grad( 0., 0., 0., 7. );
        grad.setColorAt( 0, base.light( 200 ) );
        grad.setColorAt( 0.8, base.lighter() );
        grad.setColorAt( 1, base );
        mBackBrush = QBrush( grad );
    }

    int Style::pixelMetric( PixelMetric metric, const QStyleOption* option,
                            const QWidget* widget ) const
    {
        int retval = QProxyStyle::pixelMetric( metric, option, widget );

        switch (metric)
        {
        case PM_MenuPanelWidth:
        case PM_MenuBarHMargin:
        case PM_MenuBarVMargin:
        case PM_ToolBarFrameWidth:
            if( isStyled( widget ) )
                retval = 1;
            break;

        case PM_MenuBarPanelWidth:
            if( isStyled( widget ) )
                retval = 0;
            break;

        default:
            break;
        }

        return retval;
    }

    void Style::drawControl( ControlElement element, const QStyleOption* option, QPainter* painter,
                             const QWidget* widget) const
    {
        if( !widget || !isStyled( widget ) )
        {
            goto drawDefault;
        }

        switch( element )
        {
        case CE_HeaderEmptyArea:
            painter->fillRect( option->rect, mBackBrush );
            break;

        case CE_HeaderSection:
        {
            painter->fillRect( option->rect, mBackBrush );

            QRect r( option->rect.right() - 3, option->rect.top() + 1,
                     2, option->rect.height() - 4 );
            qDrawShadePanel( painter, r, option->palette,
                             option->state & State_Sunken, 1,
                             &option->palette.brush( QPalette::Button ) );
            break;
        }

        case CE_HeaderLabel:
            if( const QStyleOptionHeader* h = qstyleoption_cast< const QStyleOptionHeader* >( option ) )
            {
                // this is very subtile, but does the job for now
                painter->setPen( Qt::white );
                painter->drawText( option->rect, h->text );
            }
            break;

        default:
            goto drawDefault;
        }

        return;

    drawDefault:
        QProxyStyle::drawControl( element, option, painter, widget );
    }

}
