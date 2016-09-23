#ifndef XS_COMMAND_SET_STITCH_H
#define XS_COMMAND_SET_STITCH_H

/** @file
 *  @brief TODO
 */

#include <vector>

#include "XSCommand.h"
#include "XSSquareIO.h"
#include "XSStitchTypes.h"

class XSPoint;

/**
 *  @brief TODO
 */
class XSCommandSetStitch : public XSCommand {
public:
    XSCommandSetStitch(unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent,
            StitchType stitchType, unsigned int flossIndex, bool overwrite);

    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    unsigned int m_layer;
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_xPercent;
    unsigned int m_yPercent;

public:
    StitchType m_stitchType;

private:
    unsigned int m_flossIndex;
    bool m_overwrite;
    XSSquareIO m_oldSquare;
};


/**
 *  @brief TODO
 */
class XSCommandSetStitches : public XSCommand {
public:
    XSCommandSetStitches(std::vector<XSPoint>& points, StitchType stitchType,
            unsigned int flossIndex, bool overwrite);

    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    unsigned int m_layer;
    std::vector<XSPoint> m_points;
    StitchType m_stitchType;
    unsigned int m_flossIndex;
    bool m_overwrite;
    std::vector<XSSquareIO> m_oldSquares;
};


#endif
