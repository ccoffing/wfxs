#ifndef XS_SQUARE_H
#define XS_SQUARE_H

#include "XSSquareIO.h"
#include "XSStitchTypes.h"

#include <stdint.h>


/**
 */
struct SquareData {
    uint8_t floss1;
    uint8_t floss2;
    uint8_t floss3;
    uint8_t floss4;
    StitchTypeAndLocation stitch1   : 4;
    StitchTypeAndLocation stitch2   : 4;
    StitchTypeAndLocation stitch3   : 4;
    StitchTypeAndLocation stitch4   : 4;
    /** These num_* fields track the number of the respective
     *  elements in the hashtables -- a way to optimize out many
     *  useless hashtable searches during screen redraws.
     */
    unsigned int num_knots : 4;
    unsigned int num_beads : 4;
    uint8_t pad;
} __attribute__((packed));

/**
 */
class XSSquare {
public:
    static const unsigned int MaxStitches;
    static const unsigned int MaxKnots;
    static const unsigned int MaxBeads;

    XSSquare() :
        m_squareData(0)
    {
    }

    void setStitch(StitchTypeAndLocation stitchType, unsigned int flossIndex);

    bool findUniqueStitch(StitchType stitchType, unsigned int flossIndex, unsigned int *index,
            StitchTypeAndLocation *stitch);

    unsigned int findOverlappingStitch(StitchTypeAndLocation new_stitch) const;

    static StitchTypeAndLocation rotateStitch(StitchTypeAndLocation stitch);

    static bool sameType(StitchTypeAndLocation s1, StitchType s2);

    /**
     *  @return true if the stitch(es) are the same type and same floss.  Embellishments such
     *      as knots and beads are not considered.
     */
    bool sameStitch(XSSquare &rhs);

    void clearStitchByIndex(unsigned int index);

    void Clear();

    /**
     */
    void GetSquareData(XSSquareIO *square) const;

    void SetSquareData(XSSquareIO const *square);

protected:
    union {
        uint64_t m_squareData;
        struct SquareData m_stitches;
    };
};

#endif
