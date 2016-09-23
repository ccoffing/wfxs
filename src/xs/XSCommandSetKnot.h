#ifndef XS_COMMAND_SET_KNOT_H
#define XS_COMMAND_SET_KNOT_H

#include "XSCommand.h"
#include "XSModel.h"
#include "XSStitchTypes.h"


/**
 */
class XSCommandSetKnot : public XSCommand {
public:
    XSCommandSetKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
            unsigned int flossIndex, bool overwrite);

    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    unsigned int m_layer;
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_region;
    KnotType m_knotType;
    unsigned int m_flossIndex;
    bool m_overwrite;
};

#endif
