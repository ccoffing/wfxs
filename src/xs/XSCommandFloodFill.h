#ifndef XS_COMMAND_FLOOD_FILL_H
#define XS_COMMAND_FLOOD_FILL_H

#include "XSCommand.h"
#include "XSSelection.h"
#include "XSSquareIO.h"

/** Flood-fills an area with particular stitch(es).
 */
class XSCommandFloodFill : public XSCommand {
public:
    XSCommandFloodFill(unsigned int x,
            unsigned int y,
            XSSquareIO const &newSquare);

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
    unsigned int m_x, m_y;
    XSSquareIO m_newSquare;
    XSSelection m_selection;
};

#endif
