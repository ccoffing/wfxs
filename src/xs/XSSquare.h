#ifndef XS_SQUARE_H
#define XS_SQUARE_H

#include "XSSquareIO.h"
#include "XSStitchTypes.h"

#include <cstdint>


/**
 */
struct SquareData {
    uint8_t floss1;
    uint8_t floss2;
    uint8_t floss3;
    uint8_t floss4;
    StitchTypeAndLocation stitch1 : 4;
    StitchTypeAndLocation stitch2 : 4;
    StitchTypeAndLocation stitch3 : 4;
    StitchTypeAndLocation stitch4 : 4;
    /** These num_* fields track the number of the respective
     *  elements in the hashtables -- a way to optimize out many
     *  useless hashtable searches during screen redraws.
     */
    unsigned int numKnots : 4;
    unsigned int numBeads : 4;
    uint8_t pad;
} __attribute__((packed));

/**
 */
class XSSquare {
public:
    static const unsigned int maxStitches = 4;
    static const unsigned int maxKnots = 8;
    static const unsigned int maxBeads = 8;

    XSSquare()
        : m_squareData(0)
    {
    }

    void setStitch(StitchTypeAndLocation stitchType, unsigned int flossIndex);

    bool findUniqueStitch(StitchType stitchType, unsigned int flossIndex, unsigned int* index,
            StitchTypeAndLocation* stitch);

    unsigned int findOverlappingStitch(StitchTypeAndLocation new_stitch) const;

    static StitchTypeAndLocation rotateStitch(StitchTypeAndLocation stitch);

    static bool sameType(StitchTypeAndLocation s1, StitchType s2);

    /**
     *  @return true if the stitch(es) are the same type and same floss.  Embellishments such
     *      as knots and beads are not considered.
     */
    bool sameStitch(XSSquare& rhs);

    void clearStitchByIndex(unsigned int index);

    void clear();

    /**
     */
    void getSquareData(XSSquareIO* square) const;

    void setSquareData(XSSquareIO const* square);

protected:
    union {
        uint64_t m_squareData;
        struct SquareData m_stitches;
    };
};

#endif
