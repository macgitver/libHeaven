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

#ifndef BLUESKY_COLOR_SCHEMA_HPP
#define BLUESKY_COLOR_SCHEMA_HPP

#include <QColor>

namespace BlueSky {

    enum Colors {
        clrSeparator,
        clrLtrGradientHigh,
        clrLtrGradientLow,
        clrTtbGradientHigh,
        clrTtbGradientLow,
        clrCurModeGradientHigh,
        clrCurModeGradientLow,
        clrCurModeText,
        clrCurModeTextShadow,
        clrModeText,
        clrModeTextShadow,
    };

    class ColorSchema {
    public:
        static void init();
        static QColor get(int id);
        static QColor get(int id, int alpha);
        static void set(int id, QColor clr);

    private:
        static QHash<int, QColor> sColors;
    };

}

#endif
