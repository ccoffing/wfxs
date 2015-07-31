#ifndef XS_COMMAND_SET_TOOL_H
#define XS_COMMAND_SET_TOOL_H

/** @file
 *  @brief TODO
 */

#include "XSCommand.h"
#include "XSModel.h"

/**
 *  @brief TODO
 */
class XSCommandSetTool : public XSCommand {
public:
    XSCommandSetTool();

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
};


#endif
