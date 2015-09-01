#ifndef XS_BRESENHAM_LINE_H
#define XS_BRESENHAM_LINE_H

/** @file
 *  Bresenham line-calculating algorithm.
 */

#include <algorithm>
#include <cstdlib>
#include <vector>

/**
 *  Bresenham's Line Algorithm.
 *  Calculates the line between two points, using the Bresenham algorithm.
 *  Translated from pseudo-code at:
 *  http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 *  @param[in] x0  Starting X value.
 *  @param[in] y0  Starting Y value.
 *  @param[in] x1  Ending X value.
 *  @param[in] y1  Ending Y value.
 *  @param[out] points
 */
template<class T>
void BresenhamLine(int x0, int y0, int x1, int y1, std::vector<T> &points)
{
    int rangex = abs(x1 - x0);
    int rangey = abs(y1 - y0);
    int numPoints = std::max(rangex, rangey) + 1;

    points.reserve(numPoints);

    bool steep = rangey > rangex;
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    int direction;
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        direction = -1;
    } else {
        direction = 1;
    }
    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = 0;
    int y = y0;
    int ystep = (y0 < y1) ? 1 : -1;

    for (int x = x0; x <= x1; ++x) {
        if (steep)
            points.push_back(T(y, x));
        else
            points.push_back(T(x, y));
        error += deltay;
        if (2 * error >= deltax) {
            y += ystep;
            error -= deltax;
        }
    }
}

#endif
