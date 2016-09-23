#ifndef XS_POINT_H
#define XS_POINT_H

/**
 */
class XSPoint {
public:
    XSPoint(unsigned int x, unsigned int y)
        : x(x)
        , y(y)
    {
    }

    unsigned int x;
    unsigned int y;
};

class XSRect {
public:
    XSRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
        : x(x)
        , y(y)
        , w(w)
        , h(h)
    {
    }

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
};

#endif
