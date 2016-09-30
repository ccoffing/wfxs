#ifndef XS_COMMAND_SET_STYLE_H
#define XS_COMMAND_SET_STYLE_H

/** @file
 *  @brief TODO
 */

#include "XSCommand.h"
#include "XSModel.h"

/**
 *  @brief TODO
 */
class XSCommandSetStyle : public XSCommand {
public:
    XSCommandSetStyle(enum DrawStyle);

    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;

private:
    enum DrawStyle m_newStyle, m_oldStyle;
};


#endif
