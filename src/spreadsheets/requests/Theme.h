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

    /**
     * Represents a color in the RGBA color space. This representation is designed for simplicity of conversion to/from color representations in various languages over compactness.
     */
    struct Color : public Printable
    {
    private:
        size_t bufSize = 5;
        String buf[5];
        GSheetObjectWriter owriter;
        Color &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        // The amount of red in the color as a value in the interval [0, 1].
        Color &red(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[1], "red", String(value), false, true);
        }
        // The amount of green in the color as a value in the interval [0, 1].
        Color &green(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[2], "green", String(value), false, true);
        }
        // The amount of blue in the color as a value in the interval [0, 1].
        Color &blue(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[3], "blue", String(value), false, true);
        }
        // The fraction of this color that should be applied to the pixel. That is, the final pixel color is defined by the equation:
        // pixel color = alpha * (this color) + (1.0 - alpha) * (background color)
        // This means that a value of 1.0 corresponds to a solid color, whereas a value of 0.0 corresponds to a completely transparent color.
        Color &alpha(float value)
        {
            if (value > 1)
                value = 1;
            else if (value < 0)
                value = 0;
            return setObject(buf[4], "alpha", String(value), false, true);
        }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

    /**
     * A color value.
     */
    class ColorStyle : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        ColorStyle() {}
        // Union field kind
        // RGB color. The alpha value in the Color object isn't generally supported.
        ColorStyle &rgbColor(Color value)
        {
            clear();
            jut.addObject(buf, "rgbColor", value.c_str(), false, true);
            return *this;
        }
        // Union field kind
        // When iterative calculation is enabled and successive results differ by less than this threshold value, the calculation rounds stop.
        ColorStyle &themeColor(ThemeColorType value)
        {
            if (value == THEME_COLOR_TYPE_UNSPECIFIED)
                jut.addObject(buf, "themeColor", "THEME_COLOR_TYPE_UNSPECIFIED", true, true);
            else if (value == TEXT)
                jut.addObject(buf, "themeColor", "TEXT", true, true);
            else if (value == BACKGROUND)
                jut.addObject(buf, "themeColor", "BACKGROUND", true, true);
            else if (value == ACCENT1)
                jut.addObject(buf, "themeColor", "ACCENT1", true, true);
            else if (value == ACCENT2)
                jut.addObject(buf, "themeColor", "ACCENT2", true, true);
            else if (value == ACCENT3)
                jut.addObject(buf, "themeColor", "ACCENT3", true, true);
            else if (value == ACCENT4)
                jut.addObject(buf, "themeColor", "ACCENT4", true, true);
            else if (value == ACCENT5)
                jut.addObject(buf, "themeColor", "ACCENT5", true, true);
            else if (value == ACCENT6)
                jut.addObject(buf, "themeColor", "ACCENT6", true, true);
            else if (value == LINK)
                jut.addObject(buf, "themeColor", "LINK", true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

    /**
     * A pair mapping a spreadsheet theme color type to the concrete color it represents.
     */
    class ThemeColorPair : public Printable
    {
    private:
        size_t bufSize = 3;
        String buf[3];
        GSheetObjectWriter owriter;
        GSheetJSONUtil jut;

        ThemeColorPair &setObject(String &buf_n, const String &key, const String &value, bool isString, bool last)
        {
            owriter.setObject(buf, bufSize, buf_n, key, value, isString, last);
            return *this;
        }

    public:
        ThemeColorPair() {}
        // The type of the spreadsheet theme color.
        ThemeColorPair &colorType(ThemeColorType value)
        {
            if (value == THEME_COLOR_TYPE_UNSPECIFIED)
                return setObject(buf[1], "colorType", "THEME_COLOR_TYPE_UNSPECIFIED", true, true);
            else if (value == TEXT)
                return setObject(buf[1], "colorType", "TEXT", true, true);
            else if (value == BACKGROUND)
                return setObject(buf[1], "colorType", "BACKGROUND", true, true);
            else if (value == ACCENT1)
                return setObject(buf[1], "colorType", "ACCENT1", true, true);
            else if (value == ACCENT2)
                return setObject(buf[1], "colorType", "ACCENT2", true, true);
            else if (value == ACCENT3)
                return setObject(buf[1], "colorType", "ACCENT3", true, true);
            else if (value == ACCENT4)
                return setObject(buf[1], "colorType", "ACCENT4", true, true);
            else if (value == ACCENT5)
                return setObject(buf[1], "colorType", "ACCENT5", true, true);
            else if (value == ACCENT6)
                return setObject(buf[1], "colorType", "ACCENT6", true, true);
            else if (value == LINK)
                return setObject(buf[1], "colorType", "LINK", true, true);
            return *this;
        }
        // The concrete color corresponding to the theme color type.
        ThemeColorPair &color(const ColorStyle &value) { return setObject(buf[2], "color", value.c_str(), false, true); }
        const char *c_str() const { return buf[0].c_str(); }
        size_t printTo(Print &p) const { return p.print(buf[0].c_str()); }
        void clear() { owriter.clearBuf(buf, bufSize); }
    };

}
#endif