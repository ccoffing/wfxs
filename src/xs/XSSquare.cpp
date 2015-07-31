#include "XSSquare.h"
#include "XSStitchTypes.h"

#include "clc/support/Debug.h"


const unsigned int XSSquare::MaxStitches = 4;
const unsigned int XSSquare::MaxKnots = 8;
const unsigned int XSSquare::MaxBeads = 8;

StitchTypeAndLocation XSSquare::rotateStitch(StitchTypeAndLocation stitch)
{
    static const StitchTypeAndLocation rotationTable[] = {
        /* StitchLoc_None           --> */ StitchLoc_None,
        /* StitchLoc_Full           --> */ StitchLoc_Full,
        /* StitchLoc_ThreeQuarterUL --> */ StitchLoc_ThreeQuarterUR,
        /* StitchLoc_ThreeQuarterUR --> */ StitchLoc_ThreeQuarterLR,
        /* StitchLoc_ThreeQuarterLL --> */ StitchLoc_ThreeQuarterUL,
        /* StitchLoc_ThreeQuarterLR --> */ StitchLoc_ThreeQuarterLL,
        /* StitchLoc_HalfBottom     --> */ StitchLoc_HalfTop,
        /* StitchLoc_HalfTop        --> */ StitchLoc_HalfBottom,
        /* StitchLoc_QuarterUL      --> */ StitchLoc_QuarterUR,
        /* StitchLoc_QuarterUR      --> */ StitchLoc_QuarterLR,
        /* StitchLoc_QuarterLL      --> */ StitchLoc_QuarterUL,
        /* StitchLoc_QuarterLR      --> */ StitchLoc_QuarterLL,
    };

    ASSERT((unsigned int)stitch < sizeof(rotationTable) / sizeof(StitchTypeAndLocation));

    return rotationTable[stitch];
}

bool XSSquare::sameType(StitchTypeAndLocation s1, StitchType s2)
{
    /* Arbitrary constants to categorize the various stitch types.
     * For example, 0 is full stitch, 1 are the various three-quarter stiches, etc.
     */
    static const char stitchTypes[] = {
        -1, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 1, 2, 3
    };

    ASSERT((unsigned int)s1 < sizeof(stitchTypes) / sizeof(char));
    ASSERT((unsigned int)s2 < sizeof(stitchTypes) / sizeof(char));

    return stitchTypes[s1] == stitchTypes[s2];
}

bool XSSquare::sameStitch(XSSquare &rhs)
{
    bool matched[4] = { false, false, false, false };

    // TODO
    return false;
}

bool XSSquare::findUniqueStitch(StitchType stitchType,
        unsigned int flossIndex,
        unsigned int *index,
        StitchTypeAndLocation *stitch)
{
    unsigned int count = 0;

#define CHECK_STITCH(n)                               \
    if (XSSquare::sameType(m_stitches.stitch ## n, stitchType) && \
        m_stitches.floss ## n == flossIndex) {          \
        *index = n;                                   \
        *stitch = m_stitches.stitch ## n;               \
        ++count;                                     \
    }
    CHECK_STITCH(1);
    CHECK_STITCH(2);
    CHECK_STITCH(3);
    CHECK_STITCH(4);
#undef CHECK_STITCH

    return count == 1;
}

unsigned int XSSquare::findOverlappingStitch(StitchTypeAndLocation new_stitch) const
{
    static const int stitchConflictTable[][StitchLoc_MAX] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // None
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, // Full
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }, // ThreeQuarterUL
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 }, // ThreeQuarterUR
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 }, // ThreeQuarterLL
        { 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 }, // ThreeQuarterLR
        { 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0 }, // HalfBottom
        { 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1 }, // HalfTop
        { 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 }, // QuarterUL
        { 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0 }, // QuarterUR
        { 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0 }, // QuarterLL
        { 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1 }  // QuarterLR
    };

    int existing_stitch;

#define CHECK_STITCH(n)                                   \
    existing_stitch = m_stitches.stitch ## n;               \
    if (stitchConflictTable[existing_stitch][new_stitch]) \
        return n
    CHECK_STITCH(1);
    CHECK_STITCH(2);
    CHECK_STITCH(3);
    CHECK_STITCH(4);
#undef CHECK_STITCH
    return 0;
}

void XSSquare::clearStitchByIndex(unsigned int index)
{
    if (index == 1)
        m_stitches.stitch1 = StitchLoc_None;
    else if (index == 2)
        m_stitches.stitch2 = StitchLoc_None;
    else if (index == 3)
        m_stitches.stitch3 = StitchLoc_None;
    else if (index == 4)
        m_stitches.stitch4 = StitchLoc_None;
    else
        ASSERT(0);
}

void XSSquare::setStitch(StitchTypeAndLocation stitchType, unsigned int flossIndex)
{
    if (m_stitches.stitch1 == StitchLoc_None) {
        m_stitches.stitch1 = stitchType;
        m_stitches.floss1 = flossIndex;
    } else if (m_stitches.stitch2 == StitchLoc_None) {
        m_stitches.stitch2 = stitchType;
        m_stitches.floss2 = flossIndex;
    } else if (m_stitches.stitch3 == StitchLoc_None) {
        m_stitches.stitch3 = stitchType;
        m_stitches.floss3 = flossIndex;
    } else if (m_stitches.stitch4 == StitchLoc_None) {
        m_stitches.stitch4 = stitchType;
        m_stitches.floss4 = flossIndex;
    } else {
        ASSERT(0);
    }
}

void XSSquare::Clear()
{
    m_squareData = 0;
}

void XSSquare::GetSquareData(XSSquareIO *square) const
{
    unsigned int count = 0;

    if (m_stitches.stitch1 != StitchLoc_None) {
        square->stitch[count].stitchType = m_stitches.stitch1;
        square->stitch[count].flossIndex = m_stitches.floss1;
        ++count;
    }
    if (m_stitches.stitch2 != StitchLoc_None) {
        square->stitch[count].stitchType = m_stitches.stitch2;
        square->stitch[count].flossIndex = m_stitches.floss2;
        ++count;
    }
    if (m_stitches.stitch3 != StitchLoc_None) {
        square->stitch[count].stitchType = m_stitches.stitch3;
        square->stitch[count].flossIndex = m_stitches.floss3;
        ++count;
    }
    if (m_stitches.stitch4 != StitchLoc_None) {
        square->stitch[count].stitchType = m_stitches.stitch4;
        square->stitch[count].flossIndex = m_stitches.floss4;
        ++count;
    }
    square->stitches = count;
}

void XSSquare::SetSquareData(XSSquareIO const *square)
{
    Clear();

    m_squareData = 0;
    do {
        unsigned int stitches = square->stitches;
        if (stitches == 0)
            break;
        m_stitches.stitch1 = square->stitch[0].stitchType;
        m_stitches.floss1  = square->stitch[0].flossIndex;
        if (stitches == 1)
            break;
        m_stitches.stitch2 = square->stitch[1].stitchType;
        m_stitches.floss2  = square->stitch[1].flossIndex;
        if (stitches == 2)
            break;
        m_stitches.stitch3 = square->stitch[2].stitchType;
        m_stitches.floss3  = square->stitch[2].flossIndex;
        if (stitches == 3)
            break;
        m_stitches.stitch4 = square->stitch[3].stitchType;
        m_stitches.floss4  = square->stitch[3].flossIndex;
    } while (0);

    // FIXME:  knots
}
