
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
