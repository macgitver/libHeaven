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

#include <QHash>
#include <QColor>
#include <QObject>

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
        clrModeDisabledText,
        clrModeDisabledTextShadow,
    };

    class ColorSchema : public QObject
    {
        Q_OBJECT
    private:
        ColorSchema();

    public:
        static ColorSchema& instance();

    signals:
        void changed();

    public:
        static QColor get(int id);
        static QColor get(int id, int alpha);
        void set(int id, QColor clr);

    private:
        void init();
        static ColorSchema* sInstance;
        QHash<int, QColor> mColors;
    };

}

#endif
