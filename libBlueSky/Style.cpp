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
#include <QMenuBar>
#include <QStyleOption>

#if QT_VERSION >= 0x050000
#include <qdrawutil.h>
#endif

#include "libBlueSky/Style.hpp"

namespace BlueSky
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

        QLinearGradient grad1( 0., 0., 0., 7. );
        grad1.setColorAt( 0, base.lighter( 200 ) );
        grad1.setColorAt( 0.8, base.lighter( 150 ) );
        grad1.setColorAt( 1, base );
        mBackBrushHor = QBrush( grad1 );

        QLinearGradient grad2( 0., 0., 7., 0. );
        grad2.setColorAt( 0, base.lighter( 200 ) );
        grad2.setColorAt( 0.8, base.lighter( 150 ) );
        grad2.setColorAt( 1, base );
        mBackBrushVer = QBrush( grad2 );
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
            if( isStyled( widget ) )
                retval = 1;
            break;

        case PM_ToolBarIconSize:
            return 15;

        case PM_ToolBarFrameWidth:
        case PM_ToolBarItemMargin:
        case PM_ToolBarItemSpacing:
        case PM_ToolBarExtensionExtent:
            return 0;

        case PM_MenuBarPanelWidth:
            if( isStyled( widget ) )
                retval = 0;
            break;

        default:
            break;
        }

        return retval;
    }

    void Style::drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
                                    QPainter* painter, const QWidget* widget ) const
    {
        if( !widget || !isStyled( widget ) )
        {
            goto drawDefault;
        }

        if( control == CC_ToolButton )
        {
            if( const QStyleOptionToolButton* buttOpt =
                    qstyleoption_cast< const QStyleOptionToolButton* >( option ) )
            {
                if( widget->parentWidget()->property( "heavenMultiBarTool" ).toBool() )
                {
                    QStyleOptionToolButton opt = *buttOpt;
                    opt.palette.setColor( QPalette::ButtonText,
                                          opt.palette.color( QPalette::BrightText ) );
                    QProxyStyle::drawComplexControl( control, &opt, painter, widget );
                    return;
                }
            }
        }

    drawDefault:
        QProxyStyle::drawComplexControl( control, option, painter, widget );
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
            painter->fillRect( option->rect, mBackBrushHor );

            painter->setPen( Qt::black );
            painter->drawLine( option->rect.bottomLeft(), option->rect.bottomRight() );
            break;

        case CE_HeaderSection:
        {
            painter->fillRect( option->rect, mBackBrushHor );

            QRect r( option->rect.right() - 3, option->rect.top() + 1,
                     2, option->rect.height() - 4 );
            qDrawShadePanel( painter, r, option->palette,
                             option->state & State_Sunken, 1,
                             &option->palette.brush( QPalette::Button ) );

            painter->setPen( Qt::black );
            painter->drawLine( option->rect.bottomLeft(), option->rect.bottomRight() );
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

        case CE_ToolBar:
            if( widget && !widget->property( "heavenMultiBarTool" ).toBool() )
            {
                proxy()->drawPrimitive( PE_PanelToolBar, option, painter, widget );
            }
            break;

        default:
            goto drawDefault;
        }

        return;

    drawDefault:
        QProxyStyle::drawControl( element, option, painter, widget );
    }

    void Style::drawPrimitive( PrimitiveElement element, const QStyleOption* option,
                               QPainter* painter, const QWidget* widget ) const
    {
        QColor base( 0x40, 0x40, 0x40 );

        if( !widget || !isStyled( widget ) )
        {
            goto drawDefault;
        }

        switch( element )
        {
        case PE_PanelStatusBar:
            {
                QLinearGradient grad( 0., 0., 100 /* option->rect.width() */, 0. );
                grad.setColorAt( 0., base.darker() );
                grad.setColorAt( 1., base.lighter( 125 ) );

                QBrush br( grad );
                painter->fillRect( option->rect, br );
                painter->setPen( Qt::black );
                painter->drawLine( option->rect.topLeft(), option->rect.topRight() );
            }
            break;

        case PE_PanelToolBar:
            if( option->state & QStyle::State_Horizontal )
            {
                painter->fillRect( option->rect, mBackBrushHor );

                painter->setPen( Qt::black );
                painter->drawLine( option->rect.topLeft(), option->rect.topRight() );
                painter->drawLine( option->rect.bottomLeft(), option->rect.bottomRight() );
            }
            else
            {
                painter->fillRect( option->rect, mBackBrushVer );

                painter->setPen( Qt::black );
                painter->drawLine( option->rect.topLeft(), option->rect.bottomLeft() );
                painter->drawLine( option->rect.topRight(), option->rect.bottomRight() );
            }
            break;

        case PE_FrameButtonTool:
            break;

        case PE_PanelButtonTool:
            if( option->state & QStyle::State_Sunken )
            {
                painter->fillRect( option->rect, QColor( 63, 63, 63, 100 ) );
            }
            else if( option->state & QStyle::State_Raised )
            {
                painter->fillRect( option->rect, QColor( 255, 255, 255, 30 ) );
            }
            break;

        case PE_FrameStatusBarItem:
            {
                QBrush br( base.lighter( 300 ) );
                painter->fillRect( option->rect, br );
                painter->setPen( base.darker() );
                painter->drawRect( option->rect );
            }
            break;

        default:
            goto drawDefault;
        }
        return;

    drawDefault:
        QProxyStyle::drawPrimitive( element, option, painter, widget );
    }

}
