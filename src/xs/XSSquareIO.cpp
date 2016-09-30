#include "XSSquare.h"
#include "XSDataIO.h"
#include "XSException.h"
#include "XSSquareIO.h"

#include <cassert>


void XSSquareIO::serialize(std::ostream& file) const
{
    /*
     *  RLE encoding elsewhere depends on the high bit of the first byte
     *  to not be set.
     */
    assert((stitches & 0x80) == 0);
    Write8_exc(file, stitches);
    for (unsigned int i = 0; i < stitches; ++i) {
        Write8_exc(file, stitch[i].flossIndex);
        Write8_exc(file, stitch[i].stitchType);
    }
    Write8_exc(file, knots | (beads << 4));
    for (unsigned int i = 0; i < knots; ++i) {
        Write8_exc(file, knot[i].flossIndex);
        Write8_exc(file, knot[i].region | (knot[i].knotType << 4));
    }
    for (unsigned int i = 0; i < beads; ++i) {
        Write8_exc(file, bead[i].colorIndex);
        Write8_exc(file, bead[i].region);
    }
}

void XSSquareIO::unserialize(std::istream& file)
{
    uint8_t ui8, b;

    Read8_exc(file, ui8);
    if (ui8 > XSSquare::maxStitches)
        goto fail;
    stitches = ui8;
    for (unsigned int i = 0; i < ui8; ++i) {
        Read8_exc(file, b);
        // FIXME:  validate this is a valid floss index
        stitch[i].flossIndex = b;
        Read8_exc(file, b);
        if (b >= StitchLoc_MAX)
            goto fail;
        stitch[i].stitchType = (StitchTypeAndLocation)b;
    }
    Read8_exc(file, ui8);
    if ((knots = ui8 & 0xf) > XSSquare::maxKnots)
        goto fail;
    if ((beads = (ui8 >> 4) & 0xf) > XSSquare::maxBeads)
        goto fail;
    for (unsigned int i = 0; i < knots; ++i) {
        Read8_exc(file, b);
        // FIXME:  validate this a valid floss index
        knot[i].flossIndex = b;
        Read8_exc(file, b);
        if (b > 9)  // FIXME: magic
            goto fail;
        knot[i].region = b & 0xf;
        knot[i].knotType = (KnotType)((b >> 4) & 0xf);
        if (knot[i].knotType >= Knot_MAX)
            goto fail;
    }
    for (unsigned int i = 0; i < beads; ++i) {
        Read8_exc(file, b);
        // FIXME:  validate this a valid color index
        bead[i].colorIndex = b;
        Read8_exc(file, b);
        if (b > 9)  // FIXME: magic
            goto fail;
        bead[i].region = b;
    }
    return;

fail:
    throw IllegalFormatException();
}
