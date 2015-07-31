#ifndef XS_COMMAND_SET_BEAD_H
#define XS_COMMAND_SET_BEAD_H

#include "XSCommand.h"

/**
 */
class XSCommandSetBead : public XSCommand {
public:
    XSCommandSetBead(unsigned int x,
            unsigned int y,
            unsigned int region,
            unsigned int colorIndex,
            bool overwrite);

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
    unsigned int m_layer;
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_region;
    unsigned int m_colorIndex;
    bool m_overwrite;
};

#endif
