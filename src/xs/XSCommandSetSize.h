#ifndef XS_COMMAND_SET_SIZE_H
#define XS_COMMAND_SET_SIZE_H

#include "XSCommand.h"

/**
 */
class XSCommandSetSize : public XSCommand {
public:
    XSCommandSetSize(unsigned int x,
            unsigned int y);

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
    unsigned int m_newX, m_newY;
    unsigned int m_oldX, m_oldY;
};

#endif
