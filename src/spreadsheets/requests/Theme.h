#ifndef THEME_H
#define THEME_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

/**
 * COLOR STYLE CLASS DEPENDENCIES
 *
 * ColorStyle           +       Color
                        |
                        +       (enum) ThemeColorType
*/

namespace GSHEET
{

    // Theme color types.
    // SpreadsheetProperties contain a SpreadsheetTheme that defines a mapping of these theme color types to concrete colors.
    enum ThemeColorType
    {
        THEME_COLOR_TYPE_UNSPECIFIED, //	Unspecified theme color
        TEXT,                         //	Represents the primary text color
        BACKGROUND,                   //	Represents the primary background color
        ACCENT1,                      //	Represents the first accent color
        ACCENT2,                      //	Represents the second accent color
        ACCENT3,                      //	Represents the third accent color
        ACCENT4,                      //	Represents the fourth accent color
        ACCENT5,                      //	Represents the fifth accent color
        ACCENT6,                      //	Represents the sixth accent color
        LINK                          //	Represents the color to use for hyperlinks
    };

    const struct key_str_30 _ThemeColorType[ThemeColorType::LINK + 1] PROGMEM = {"THEME_COLOR_TYPE_UNSPECIFIED", "TEXT", "BACKGROUND", "ACCENT1", "ACCENT2", "ACCENT3", "ACCENT4", "ACCENT5", "ACCENT6", "LINK"};

    /**
     * Represents a color in the RGBA color space. This representation is designed for simplicity of conversion to/from color representations in various languages over compactness.
     */
    struct Color : public BaseG6
    {

    public:
        // The amount of red in the color as a value in the interval [0, 1].
        Color &red(float value) { return wr.set<Color &, float>(*this, GSheetUnityRange().val(value), buf, bufSize, 1, FPSTR(__func__)); }

        // The amount of green in the color as a value in the interval [0, 1].
        Color &green(float value) { return wr.set<Color &, float>(*this, GSheetUnityRange().val(value), buf, bufSize, 2, FPSTR(__func__)); }

        // The amount of blue in the color as a value in the interval [0, 1].
        Color &blue(float value) { return wr.set<Color &, float>(*this, GSheetUnityRange().val(value), buf, bufSize, 3, FPSTR(__func__)); }

        // The fraction of this color that should be applied to the pixel. That is, the final pixel color is defined by the equation:
        // pixel color = alpha * (this color) + (1.0 - alpha) * (background color)
        // This means that a value of 1.0 corresponds to a solid color, whereas a value of 0.0 corresponds to a completely transparent color.
        Color &alpha(float value) { return wr.set<Color &, float>(*this, GSheetUnityRange().val(value), buf, bufSize, 4, FPSTR(__func__)); }
    };

    /**
     * A color value.
     */
    class ColorStyle : public BaseG1
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        ColorStyle() = default;

        // Union field kind
        // RGB color. The alpha value in the Color object isn't generally supported.
        ColorStyle &rgbColor(Color value) { return wr.add<ColorStyle &, Color>(*this, value, buf, FPSTR(__func__)); }

        // Union field kind
        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        ColorStyle &themeColor(ThemeColorType value) { return wr.add<ColorStyle &, const char *>(*this, _ThemeColorType[value].text, buf, FPSTR(__func__)); }
    };

    /**
     * A pair mapping a spreadsheet theme color type to the concrete color it represents.
     */
    class ThemeColorPair : public BaseG4
    {

    public:
        ThemeColorPair() = default;

        // The type of the spreadsheet theme color.
        ThemeColorPair &colorType(ThemeColorType value) { return wr.set<ThemeColorPair &, const char *>(*this, _ThemeColorType[value].text, buf, bufSize, 1, FPSTR(__func__)); }

        // The concrete color corresponding to the theme color type.
        ThemeColorPair &color(const ColorStyle &value) { return wr.set<ThemeColorPair &, ColorStyle>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }
    };

}
#endif