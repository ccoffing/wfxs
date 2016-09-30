#ifndef XS_COMMAND_SET_SIZE_H
#define XS_COMMAND_SET_SIZE_H

#include "XSCommand.h"

/**
 */
class XSCommandSetSize : public XSCommand {
public:
    XSCommandSetSize(unsigned int x, unsigned int y);

    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;

private:
    unsigned int m_newX, m_newY;
    unsigned int m_oldX, m_oldY;
};

#endif
