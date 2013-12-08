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

#define STYLE_RED_WINE
#define STYLE_BLUE_SKY

#include <QHash>

#include "libBlueSky/ColorSchema.hpp"

namespace BlueSky {

    ColorSchema::ColorSchema() {
        init();
    }

    ColorSchema& ColorSchema::instance() {
        if (!sInstance) {
            sInstance = new ColorSchema;
        }
        return *sInstance;
    }

    ColorSchema* ColorSchema::sInstance = NULL;

    void ColorSchema::init() {
        #if defined(STYLE_BLUE_SKY)
        set(clrSeparator,           qRgb(0x00, 0x3F, 0x5F));
        set(clrLtrGradientLow,      qRgb(0x8F, 0xAF, 0xCF));
        set(clrLtrGradientHigh,     qRgb(0x4F, 0x6F, 0x8F));
        set(clrTtbGradientHigh,     qRgb(0x8F, 0xAF, 0xCF));
        set(clrTtbGradientLow,      qRgb(0x4F, 0x6F, 0x8F));
        set(clrCurModeGradientHigh, qRgb(0xBF, 0xDF, 0xFF));
        set(clrCurModeGradientLow,  qRgb(0x7F, 0x9F, 0xBF));
        set(clrModeText,            qRgb(0xBF, 0xDF, 0xFF));
        set(clrModeTextShadow,      qRgb(0x00, 0x3F, 0x5F));
        set(clrCurModeText,         qRgb(0x00, 0x3F, 0x5F));
        set(clrCurModeTextShadow,   qRgb(0xBF, 0xDF, 0xFF));
        #elif defined(STYLE_RED_WINE)
        // "Red Wine"
        set(clrSeparator,           qRgb(0x5F, 0x00, 0x3F));
        set(clrLtrGradientLow,      qRgb(0xCF, 0x8F, 0xAF));
        set(clrLtrGradientHigh,     qRgb(0x8F, 0x4F, 0x6F));
        set(clrTtbGradientHigh,     qRgb(0xCF, 0x8F, 0xAF));
        set(clrTtbGradientLow,      qRgb(0x8F, 0x4F, 0x6F));
        set(clrCurModeGradientHigh, qRgb(0xFF, 0xBF, 0xDF));
        set(clrCurModeGradientLow,  qRgb(0xBF, 0x7F, 0x9F));
        set(clrModeText,            qRgb(0xFF, 0xBF, 0xDF));
        set(clrModeTextShadow,      qRgb(0x5F, 0x00, 0x3F));
        set(clrCurModeText,         qRgb(0x5F, 0x00, 0x3F));
        set(clrCurModeTextShadow,   qRgb(0xFF, 0xBF, 0xDF));
        #endif
    }

    QColor ColorSchema::get(int id) {
        return instance().mColors.value(id, Qt::white);
    }

    QColor ColorSchema::get(int id, int alpha) {
        QColor c = instance().mColors.value(id, Qt::white);
        c.setAlpha(alpha);
        return c;
    }

    void ColorSchema::set(int id, QColor clr) {
        mColors.insert(id, clr);
        emit changed();
    }

}
