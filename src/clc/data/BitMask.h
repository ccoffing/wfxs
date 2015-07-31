#ifndef LIBCLC_DATA_BITMASK_H
#define LIBCLC_DATA_BITMASK_H

#include "clc/data/BitString.h"

#include <assert.h>


namespace clc {

/**
 *  A bitmask.
 *
 *  The mask supports various operations (cropping, inverting).
 *  @todo get size
 */
class BitMask {
public:
    /**
     *  Creates a bitmask of the specified size.
     *  All bits default to cleared.
     *  @param[in] x    The width; must be > 0.
     *  @param[in] y    The height; must be > 0.
     */
    BitMask(unsigned int x, unsigned int y);

    /**
     *  Creates a bitmask, by copying another.
     */
    BitMask(BitMask const &);

    /**
     *  Clears all bits.
     */
    inline void clear()
    {
        m_bs.clear();
    }

    /**
     *  Sets all bits.
     */
    inline void set()
    {
        m_bs.set();
    }

    /** Returns nonzero if the bit at (x,y) is set. */
    bool test(unsigned int x, unsigned int y) const;

    /** Sets the bit at (x,y) */
    void set(unsigned int x, unsigned int y);

    /** Clears the bit at (x,y) */
    void clear(unsigned int x, unsigned int y);

    /**
     *  Inverts each element (binary NOT) in the mask.
     */
    inline void invert()
    {
        m_bs.invert();
    }

    /**
     *  Performs a floodfill, starting at (x,y).  If fill, sets squares,
     *  stopping at boundaries that are already set.  If not fill, clears
     *  squares, stopping at boundaries that are already cleared.  Does not
     *  flood diagonally.
     */
    void FloodFill(unsigned int x, unsigned int y, bool fill);

    /** Erases mask b from mask a (a &= ~b) */
    void Erase(BitMask const &b, unsigned int xoffset = 0, unsigned int yoffset = 0);

    /**
     *  Crops the mask.
     *  @param[in] left    Amount to shift left side.
     *  @param[in] top     Amount to shift top boundary.
     *  @param[in] right   Amount to shift right boundary.
     *  @param[in] bottom  Amount to shift bottom boundary.
     *  @param[in] fill
     */
    bool Crop(int left, int top, int right, int bottom, bool fill = false);

private:
    inline unsigned int offset(unsigned int x, unsigned int y) const
    {
        assert(x < w);
        assert(y < h);
        return y * w + x;
    }

    unsigned int w, h;
    BitString m_bs;
};
}

#endif
