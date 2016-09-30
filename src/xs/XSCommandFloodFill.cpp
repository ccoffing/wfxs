#include "BitMask.h"
#include "XSCommandFloodFill.h"
#include "XSModel.h"
#include "XSSquare.h"


XSCommandFloodFill::XSCommandFloodFill(unsigned int x, unsigned int y, XSSquareIO const& newSquare)
    : m_x(x)
    , m_y(y)
    , m_newSquare(newSquare)
    , m_selection()
{
}

int XSCommandFloodFill::doCommand()
{
    unsigned int dx = m_model->squaresX();
    unsigned int dy = m_model->squaresY();
    unsigned int layer = m_model->getCurrentLayerIndex();
    XSSquare oldSquare = m_model->getSquare(m_x, m_y, layer);
    BitMask bitmask(dx, dy);
    BitMask flooded(bitmask);

    // If this is a performance bottleneck, take the BitMask.floodFill algorithm and pull it
    // out and templatize it, so it can directly operate on XSSquare.
    // Set up the boundaries
    for (unsigned int y = 0; y < dy; ++y) {
        for (unsigned int x = 0; x < dx; ++x) {
            // Set differing stitches; these are the boundary.
            if (!m_model->getSquare(x, y, layer).sameStitch(oldSquare))
                flooded.set(x, y);
        }
    }

    // Do the floodfill.
    flooded.floodFill(m_x, m_y, true);

    for (unsigned int y = 0; y < dy; ++y) {
        for (unsigned int x = 0; x < dx; ++x) {
            if (bitmask.test(x, y) != flooded.test(x, y))
                // TODO:  this blows away embellishments!  Only want to floodfill stitches.
                m_model->setSquareDataNoInval(&m_newSquare, x, y, layer);
            // TODO:  Save the old stitch in a ... ?  XSSelectionRectangle?
        }
    }
    // TODO:  invalidate the entire changed region once

    return true;
}

int XSCommandFloodFill::undoCommand()
{
    // FIXME   restore the XSSelectionRectangle
    // TODO:  this suggests that the XSLayer shoudl have a paste API
    return false;
}

char const* XSCommandFloodFill::getDescription() const
{
    return "flood fill";
}
