#ifndef XS_COMMAND_CLEAR_SQUARE_H
#define XS_COMMAND_CLEAR_SQUARE_H

#include "XSCommand.h"
#include "XSSquareIO.h"

#include <vector>

class XSPoint;

/** Command to clear a cross-stitch square.
 */
class XSCommandClearSquare : public XSCommand {
public:
    XSCommandClearSquare(unsigned int x, unsigned int y, unsigned int layer);
    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    unsigned int m_layer;
    unsigned int m_x, m_y;
    XSSquareIO m_oldSquare;
};


/** Command to clear multiple cross-stitch squares.
 */
class XSCommandClearSquares : public XSCommand {
public:
    XSCommandClearSquares(std::vector<XSPoint> const& points, unsigned int layer);

    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    unsigned int m_layer;
    std::vector<XSPoint> m_points;
    std::vector<XSSquareIO> m_oldSquares;
};

#endif
