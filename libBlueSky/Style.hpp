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

#ifndef MGV_HEAVEN_STYLE_H
#define MGV_HEAVEN_STYLE_H

#include <QProxyStyle>

namespace BlueSky
{

    class Style : public QProxyStyle
    {
        Q_OBJECT
    public:
        Style( QStyle* baseStyle );

    public:
        int pixelMetric( PixelMetric metric, const QStyleOption* option,
                         const QWidget* widget ) const;

        void drawControl( ControlElement element, const QStyleOption* option, QPainter* painter,
                          const QWidget* widget ) const;

        void drawPrimitive( PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                            const QWidget* widget ) const;

        void drawComplexControl( ComplexControl control, const QStyleOptionComplex* option,
                                 QPainter* painter, const QWidget* widget ) const;

    private:
        QBrush mBackBrushHor;
        QBrush mBackBrushVer;
    };

}

#endif
