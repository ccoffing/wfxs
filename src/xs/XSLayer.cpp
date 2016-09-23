#include "XSLayer.h"
#include "XSSquare.h"
#include "XSStitchTypes.h"

#include <assert.h>

XSLayer::XSLayer(unsigned int sizeX, unsigned int sizeY)
    : m_sizeX(sizeX)
    , m_sizeY(sizeY)
    , m_grid(new XSSquare[sizeX * sizeY])
    , m_embelishments()
{
}

XSLayer::XSLayer(const XSLayer& rhs)
    : m_sizeX(rhs.m_sizeX)
    , m_sizeY(rhs.m_sizeY)
    , m_grid(new XSSquare[m_sizeX * m_sizeY])
    , m_embelishments(rhs.m_embelishments)
{
    for (unsigned int i = 0; i < m_sizeX * m_sizeY; ++i) {
        m_grid[i] = rhs.m_grid[i];
    }
}

XSLayer::~XSLayer()
{
    delete[] m_grid;
}

XSSquare* XSLayer::getSquare(unsigned int x, unsigned int y) const
{
    return m_grid + (m_sizeX * y) + x;
}

void XSLayer::Resize(unsigned int sizeX, unsigned int sizeY)
{
    if (sizeX == m_sizeX && sizeY == m_sizeY)
        return;

    XSSquare* grid = new XSSquare[sizeX * sizeY];

    // Copy over existing squares.
    unsigned int minX = (sizeX < m_sizeX ? sizeX : m_sizeX);
    unsigned int minY = (sizeY < m_sizeY ? sizeY : m_sizeY);
    for (unsigned int y = 0; y < minY; ++y) {
        for (unsigned int x = 0; x < minX; ++x) {
            grid[(sizeX * y) + x] = m_grid[(m_sizeX * y) + x];
        }
    }

    // Release no-longer-used squares.
    delete[] m_grid;

    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_grid = grid;
}

/*
 *  If a stitch type with an explicit orientation is selected (i.e., not
 *  a  *Auto stitch) then it will over-write any existing stitches (assuming
 *  overwrite mode, of course).
 *
 *  But that's not this function.
 *
 *  A *Auto stitch will NOT overwrite.  It will do one of two things:
 *    - If a stitch of that type (quarter, half, or three-quarter) already
 *      exists in the square, it will rotate the existing stitch, if possible.
   FIXME -- rotate if clicked on existing stitch; otherwise add new elsewhere
 *    - Otherwise, it will add a new stitch, oriented based on:
 *        1.  What corner the user clicked
 *        2.  If that would conflict with other existing stitches, it will
 *            auto-rotate the stitch to make it fit.  If it can't fit in
 *            any orientation, it does nothing.
 *
 *  Returns the stitch that was actually used (n.b., it won't return an
 *  *Auto stitch; it will return the actual orientation used.)
 */

StitchTypeAndLocation XSLayer::autoStitch(unsigned int squareX, unsigned int squareY,
        unsigned int xPercent, unsigned int yPercent, StitchType stitchType,
        unsigned int flossIndex)
{
    assert(stitchType == Stitch_QuarterAuto || stitchType == Stitch_HalfAuto
            || stitchType == Stitch_ThreeQuarterAuto);

    XSSquare* square = getSquare(squareX, squareY);

    unsigned int rotationsTried;
    unsigned int i;
    StitchTypeAndLocation stitchToRotate;
    if (square->findUniqueStitch(stitchType, flossIndex, &i, &stitchToRotate)) {
        square->clearStitchByIndex(i);

        rotationsTried = 0;
        for (StitchTypeAndLocation rotated = XSSquare::rotateStitch(stitchToRotate);
                rotationsTried < 3; rotated = XSSquare::rotateStitch(rotated), ++rotationsTried) {
            if (!square->findOverlappingStitch(rotated)) {
                square->setStitch(rotated, flossIndex);
                return rotated;
            }
        }

        // Rotation didn't work -- put old stitch back.
        square->setStitch(stitchToRotate, flossIndex);
    }

    // Existing stich couldn't be rotated, or there was no existing stitch.
    // Try to add a new one.
    if (stitchType == Stitch_ThreeQuarterAuto || stitchType == Stitch_QuarterAuto) {
        unsigned int offset = 0;
        // WARNING:  I depend on a UL, UR, LL, LR ordering in the enum.
        // FIXME:  Use region; do not pass % in here.
        if (yPercent > 50)
            offset += 2;
        if (xPercent > 50)
            offset++;

        if (stitchType == Stitch_ThreeQuarterAuto)
            stitchToRotate = StitchLoc_ThreeQuarterUL;
        else
            stitchToRotate = StitchLoc_QuarterUL;
        stitchToRotate = (StitchTypeAndLocation)(stitchToRotate + offset);
    } else {
        assert(stitchType == Stitch_HalfAuto);
        if (yPercent > 50)
            stitchToRotate = StitchLoc_HalfBottom;
        else
            stitchToRotate = StitchLoc_HalfTop;
    }

    rotationsTried = 0;
    for (StitchTypeAndLocation rotated = stitchToRotate; rotationsTried < 3;
            rotated = XSSquare::rotateStitch(rotated), ++rotationsTried) {
        if (!square->findOverlappingStitch(rotated)) {
            square->setStitch(rotated, flossIndex);
            return rotated;
        }
    }
    return StitchLoc_None;
}

void XSLayer::replaceStitch(unsigned int squareX, unsigned int squareY,
        StitchTypeAndLocation stitchType, unsigned int flossIndex, bool overwrite)
{
    XSSquare* square = getSquare(squareX, squareY);

    if (overwrite) {
        int i;
        while ((i = square->findOverlappingStitch(stitchType))) {
            square->clearStitchByIndex(i);
        }
    } else {
        if (square->findOverlappingStitch(stitchType))
            return;
    }

    square->setStitch(stitchType, flossIndex);
}

StitchType XSLayer::SetStitch(unsigned int squareX, unsigned int squareY, unsigned int xPercent,
        unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite)
{
    if (stitchType == Stitch_QuarterAuto || stitchType == Stitch_HalfAuto
            || stitchType == Stitch_ThreeQuarterAuto) {
        StitchTypeAndLocation stl;
        stl = autoStitch(squareX, squareY, xPercent, yPercent, stitchType, flossIndex);
        /*
         *  If an orientation was found, then return that; if it wasn't possible
         *  to place the stitch, return what was requested (possibly an*Auto
         *  stitch, so that the orientation might still "lock in" later when
         *  dragged to the next square.)
         */
        if (stl != StitchLoc_None)
            stitchType = (StitchType)stl;
    } else {
        replaceStitch(squareX, squareY, (StitchTypeAndLocation)stitchType, flossIndex, overwrite);
    }
    return stitchType;
}

void XSLayer::SetKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
        unsigned int flossIndex, bool overwrite)
{
    SetEmbelishment(EmbelType_Knot, x, y, region, knotType, flossIndex, overwrite);
}

void XSLayer::SetBead(unsigned int x, unsigned int y, unsigned int region, unsigned int colorIndex,
        bool overwrite)
{
    SetEmbelishment(
            EmbelType_Bead, x, y, region, (KnotType)0 /* FIXME - yuck*/, colorIndex, overwrite);
}

void XSLayer::SetEmbelishment(EmbelType type, unsigned int x, unsigned int y, unsigned int region,
        KnotType knotType, unsigned int index, bool overwrite)
{
    bool exists;
    EmbLocation embloc;

    assert(x < m_sizeX);
    assert(y < m_sizeY);
    assert(region < 10 && region != 2 && region != 7);

    embloc.x = x;
    embloc.y = y;
    embloc.region = region;

    if (!overwrite) {
        exists = (m_embelishments.count(embloc) != 0);
        if (exists)
            return;
    } else {
        // Optimization:  Assume it exists.  Even if it didn't, I'd have to make one
        // hash lookup to determine that.  So assume it does, and try to remove it.
        exists = true;
    }

    if (exists)
        m_embelishments.erase(embloc);

    Embelishment emb;
    emb.m_embType = type;
    if (type == EmbelType_Knot) {
        emb.knot.m_knotType = knotType;
        emb.knot.m_flossIndex = index;
    } else if (type == EmbelType_Bead) {
        emb.bead.m_colorIndex = index;
    } else {
        assert(0);
    }
    m_embelishments.insert(std::pair<EmbLocation, Embelishment>(embloc, emb));

    assert(m_embelishments.count(embloc) == 1);
}

void XSLayer::ClearSquare(unsigned int x, unsigned int y)
{
    assert(x < m_sizeX && y < m_sizeY);
    XSSquare* square = getSquare(x, y);
    square->Clear();
}

void XSLayer::Backstitch()
{
    // FIXME
}

void XSLayer::GetSquareData(XSSquareIO* square, unsigned int squareX, unsigned int squareY)
{
    assert(square);
    assert(squareX < m_sizeX);
    assert(squareY < m_sizeY);

    XSSquare* s = getSquare(squareX, squareY);

    s->GetSquareData(square);

    EmbLocation embloc;
    embloc.x = squareX;
    embloc.y = squareY;
    square->knots = 0;
    square->beads = 0;
    for (unsigned int i = 0; i < 13; ++i) {  // FIXME:  don't hardcode 13 regions
        embloc.region = i;
        std::map<EmbLocation, Embelishment>::iterator it = m_embelishments.find(embloc);
        if (it == m_embelishments.end())
            continue;
        Embelishment const emb = (*it).second;
        if (emb.m_embType == EmbelType_Knot) {
            square->knot[square->knots].region = i;
            square->knot[square->knots].knotType = emb.knot.m_knotType;
            square->knot[square->knots].flossIndex = emb.knot.m_flossIndex;
            square->knots++;
        } else {
            assert(emb.m_embType == EmbelType_Bead);
            square->bead[square->beads].region = i;
            square->bead[square->beads].colorIndex = emb.bead.m_colorIndex;
            square->beads++;
        }
    }
}

void XSLayer::SetSquareData(XSSquareIO const* square, unsigned int x, unsigned int y)
{
    assert(square);
    assert(x < m_sizeX);
    assert(y < m_sizeY);

    XSSquare* s = getSquare(x, y);
    s->SetSquareData(square);
}
