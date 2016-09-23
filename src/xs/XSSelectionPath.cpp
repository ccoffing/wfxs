#include "XSSelection.h"
#include "BitMask.h"
#include "XSSelectionPath.h"

#include <algorithm>
#include <vector>


void XSSelectionPath::Move(MoveDirection move, int* x, int* y)
{
    switch (move) {
    case UP:
        (*y)--;
        break;
    case DOWN:
        (*y)++;
        break;
    case LEFT:
        (*x)--;
        break;
    case RIGHT:
        (*x)++;
        break;
    default:
        assert(0);
    }
}

XSFreeFormSelectionPath::XSFreeFormSelectionPath()
    : m_moves()
    , m_width(0)
    , m_height(0)
    , m_x(0)
    , m_y(0)
    , m_complete(false)
    , m_validCache(true)
{
}

BitMask* XSFreeFormSelectionPath::GetMask()
{
    UpdateCache();
    if (!m_complete)
        return 0;

    // At this point, the path is known to be complete, but how to find the inside of it?  Since
    // it is complete, I have a stable frame of reference (a stable origin), and I have a
    // height, and I know at some point the path must come UP to the top edge and then turn LEFT
    // or RIGHT.  This gives me a starting point to trace the outside in the mask.  Then
    // floodfill the mask from one point down from the starting outline point.  Since the path
    // can't touch itself, the floodfill will get everywhere.

    assert(m_width > 0);
    assert(m_height > 0);
    BitMask* mask = new BitMask(m_width + 2, m_height + 2);
    bool tracing = false;
    int x = m_x1, y = m_y1;  // Offsets at start
    int clockwise = 0, flood_x = -1, flood_y = 0;
    std::vector<MoveDirection>::iterator it = m_moves.begin();
    while (1) {
        // Once path moves along the top, I know where the interior is and can begin tracing.
        if (!tracing && y == 0 && *it != DOWN) {
            tracing = true;
            if (*it == LEFT) {
                flood_x = x - 1;
                clockwise = 0;
            } else {
                assert(*it == RIGHT);
                flood_x = x;
                clockwise = 1;
            }
        }
        XSSelectionPath::Move(*it, &x, &y);
        if (tracing) {
            // Beware, premature optimization is fun.  How do we decide which square to
            // mark for the outline, considering the current direction and whether we
            // started clockwise?  Convert direction and clockwise into array index.
            // LEFT=-1, RIGHT=1, UP=-2, DOWN=2
            unsigned int i = ((*it) + 2) + (clockwise * 5);
            // Offsets are for when going...
            //   ccw UP, ccw LEFT, (unused), ccw RIGHT, ccw DOWN,
            //   cw  UP, cw  LEFT, (unused), cw  RIGHT, cw  DOWN
            int const x_offsets[] = { 0, 0, 99, -1, -1, -1, 0, 99, -1, 0 };
            int const y_offsets[] = { 0, -1, 99, 0, -1, 0, 0, 99, -1, -1 };
            unsigned int bx = x + x_offsets[i];
            unsigned int by = y + y_offsets[i];
            if (mask->test(bx, by))
                break;
            mask->set(bx, by);
        }
        ++it;
        if (it == m_moves.end())
            it = m_moves.begin();
    }
    assert(flood_x > 0);
    BitMask outline = *mask;
    mask->FloodFill(flood_x, flood_y, true);
    mask->Erase(outline);
    mask->Crop(1, 1, -1, -1);
    return mask;
}

bool XSFreeFormSelectionPath::Move(MoveDirection move)
{
    UpdateCache();
    if (m_complete)
        return false;

    if (!m_moves.empty()) {
        // Backtracking is special -- allowed, but cancels out
        if (-move == m_moves.back()) {
            m_moves.pop_back();
            m_validCache = false;
            return true;
        }

        // Disallow touching previous path, other than re-joining to the starting point
        int new_x = m_x, new_y = m_y;
        XSSelectionPath::Move(move, &new_x, &new_y);
        int x = 0, y = 0;
        std::vector<MoveDirection>::iterator it = m_moves.begin();
        do {
            XSSelectionPath::Move(*it, &x, &y);
            ++it;
            if (new_x == x && new_y == y && it != m_moves.end())
                return false;
        } while (it != m_moves.end());
    }
    m_moves.push_back(move);
    m_validCache = false;
    return true;
}

bool XSFreeFormSelectionPath::IsCompleted()
{
    UpdateCache();
    return m_complete;
}

bool XSFreeFormSelectionPath::Complete()
{
    assert(0);  // FIXME -- how to do this without crossing?
    return false;
}

void XSFreeFormSelectionPath::GetDimensions(unsigned int* width, unsigned int* height)
{
    UpdateCache();
    *width = m_width;
    *height = m_height;
}

void XSFreeFormSelectionPath::GetEnd(unsigned int* x, unsigned int* y)
{
    UpdateCache();
    *x = m_x;
    *y = m_y;
}

void XSFreeFormSelectionPath::UpdateCache()
{
    if (!m_validCache) {
        WalkPath(&m_width, &m_height, &m_x, &m_y, &m_x1, &m_y1);
        m_validCache = true;
    }
    m_complete = (m_x == 0 && m_y == 0 && !m_moves.empty());
}

void XSFreeFormSelectionPath::WalkPath(
        unsigned int* width, unsigned int* height, int* dx2, int* dy2, int* dx1, int* dy1)
{
    int x = 0, minx = 0, maxx = 0;
    int y = 0, miny = 0, maxy = 0;

    std::vector<MoveDirection>::iterator it;

    for (it = m_moves.begin(); it != m_moves.end(); ++it) {
        XSSelectionPath::Move(*it, &x, &y);
        minx = std::min(x, minx);
        maxx = std::max(x, maxx);
        miny = std::min(y, miny);
        maxy = std::max(y, maxy);
    }
    *width = maxx - minx;
    *height = maxy - miny;
    *dx2 = x;
    *dy2 = y;
    if (dx1)
        *dx1 = x - maxx;
    if (dy1)
        *dy1 = y - maxy;
}

XSRectangularSelectionPath::XSRectangularSelectionPath()
    : m_x(0)
    , m_y(0)
    , m_complete(false)
{
}

BitMask* XSRectangularSelectionPath::GetMask()
{
    if (!m_complete)
        return 0;
    return new BitMask(m_x, m_y);
}

bool XSRectangularSelectionPath::Move(MoveDirection m)
{
    XSSelectionPath::Move(m, &m_x, &m_y);

    return true;
}

void XSRectangularSelectionPath::GetDimensions(unsigned int* width, unsigned int* height)
{
    *width = (m_x < 0) ? -m_x : m_x;
    *height = (m_y < 0) ? -m_y : m_y;
}

void XSRectangularSelectionPath::GetEnd(unsigned int* x, unsigned int* y)
{
    *x = m_x;
    *y = m_y;
}

bool XSRectangularSelectionPath::IsCompleted()
{
    return m_complete;
}

bool XSRectangularSelectionPath::Complete()
{
    m_complete = true;
    return true;
}
