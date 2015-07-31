#ifndef XS_COLOR_H
#define XS_COLOR_H

#include <iostream>
#include <stdint.h>


class XSColor {
public:
    friend std::ostream &operator<<(std::ostream &out, XSColor const &c);

    XSColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xff) :
        red(red),
        green(green),
        blue(blue),
        alpha(alpha)
    {
    }

    bool operator==(XSColor const &rhs) const;

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

#endif
