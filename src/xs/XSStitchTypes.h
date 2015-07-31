#ifndef XS_STITCH_TYPES_H
#define XS_STITCH_TYPES_H


typedef enum _StitchTypeAndLocation {
    StitchLoc_None = 0,
    StitchLoc_Full,
    StitchLoc_ThreeQuarterUL,
    StitchLoc_ThreeQuarterUR,
    StitchLoc_ThreeQuarterLL,
    StitchLoc_ThreeQuarterLR,
    StitchLoc_HalfBottom,
    StitchLoc_HalfTop,
    StitchLoc_QuarterUL,
    StitchLoc_QuarterUR,
    StitchLoc_QuarterLL,
    StitchLoc_QuarterLR,
    StitchLoc_MAX
} StitchTypeAndLocation;

/**
 *  Note that the ordering matches StitchTypeAndLocation --
 *  do not break this relationship.
 *  Also, code depends on the UL, UR, LL, LR ordering.
 */
typedef enum _StitchType {
    Stitch_Full = (int)StitchLoc_Full,
    Stitch_ThreeQuarterUL,
    Stitch_ThreeQuarterUR,
    Stitch_ThreeQuarterLL,
    Stitch_ThreeQuarterLR,
    Stitch_HalfBottom,
    Stitch_HalfTop,
    Stitch_QuarterUL,
    Stitch_QuarterUR,
    Stitch_QuarterLL,
    Stitch_QuarterLR,
    Stitch_ThreeQuarterAuto,
    Stitch_HalfAuto,
    Stitch_QuarterAuto
} StitchType;

typedef enum _KnotType {
    Knot_French = 0,
    Knot_LazyDaisyKnot,
    Knot_MAX
} KnotType;

typedef enum _EmbelType {
    EmbelType_Bead = 0,
    EmbelType_Knot,
    EmbelType_MAX
} EmbelType;

#endif
