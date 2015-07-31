#ifndef XS_SQUARE_IO_H
#define XS_SQUARE_IO_H

#include <iostream>

#include "XSStitchTypes.h"


/** A data structure used to export the contents of a square.
 *  @note This structure is constructed for speed, not space efficiency.
 */
struct XSSquareIO {
    XSSquareIO() :
        stitches(0),
        knots(0),
        beads(0)
    {
    }

    inline bool isClear() const
    {
        return stitches == 0 && knots == 0 && beads == 0;
    }

    void Serialize(std::ostream &file) const;
    void Unserialize(std::istream &file);

    unsigned int stitches;
    struct {
        StitchTypeAndLocation stitchType;
        unsigned int flossIndex;
    } stitch[4];

    unsigned int knots;
    struct {
        unsigned int region;
        KnotType knotType;
        unsigned int flossIndex;
    } knot[8];

    unsigned int beads;
    struct {
        unsigned int region;
        unsigned int colorIndex;
    } bead[8];
};

#endif
