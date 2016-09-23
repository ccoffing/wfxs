#include "xs/BitMask.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


BitMask::BitMask(unsigned int x, unsigned int y)
    : w(x)
    , h(y)
    , m_bs(x * y)
{
    assert(w > 0);
    assert(h > 0);
}

BitMask::BitMask(BitMask const& rhs)
    : w(rhs.w)
    , h(rhs.h)
    , m_bs(rhs.m_bs)
{
}

bool BitMask::test(unsigned int x, unsigned int y) const
{
    return m_bs.test(offset(x, y));
}

void BitMask::set(unsigned int x, unsigned int y)
{
    m_bs.set(offset(x, y));
}

void BitMask::clear(unsigned int x, unsigned int y)
{
    m_bs.clear(offset(x, y));
}

void BitMask::FloodFill(unsigned int x, unsigned int y, bool fill)
{
    assert(x < w);
    assert(y < h);

    unsigned int i = offset(x, y);
    if (m_bs.test(i) == fill)
        return;

    std::vector<unsigned int> q;
    q.push_back(i);

    while (!q.empty()) {
        i = q.back();
        q.pop_back();
        assert(m_bs.test(i) == !fill);

        // Fill this scanline
        unsigned int start = (i / w) * w;
        unsigned int stop = start + w - 1;
        unsigned int west = m_bs.first(i, start, fill);
        if (west < start)
            west = start;
        unsigned int east = m_bs.first(i, stop, !fill);
        if (east > stop)
            east = stop;
        m_bs.change(west, east - west + 1, fill);

        // Now search the same width, one line up and one line down (if those
        // lines even exist).  Make note of the first fillable pixels while
        // scanning from left to right.
        for (int updown = -1; updown <= 1; updown += 2) {
            if ((updown == -1 && (i > w)) || (updown == 1 && (i <= (w * (h - 1))))) {
                unsigned int _west = west + (w * updown);
                unsigned int _east = east + (w * updown);
                start = _west;
                bool state = (m_bs.test(start) != fill);
                while (true) {
                    if (state)
                        q.push_back(start);
                    // Now find where state changes
                    stop = m_bs.first(start, _east, !state);
                    if (stop > _east)
                        break;  // Was no change for rest of scanline; done.
                    start = stop;
                    state = !state;
                }
            }
        }
    }
}

void BitMask::Erase(BitMask const& b, unsigned int xoffset, unsigned int yoffset)
{
    // FIXME
}

bool BitMask::Crop(int left, int top, int right, int bottom, bool fill)
{
    // FIXME
}
