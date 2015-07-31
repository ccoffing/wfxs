#ifndef XS_COMMAND_SHOW_RULERS_H
#define XS_COMMAND_SHOW_RULERS_H

/** @file
 *  @brief TODO
 */

#include "XSCommand.h"

/**
 *  @brief TODO
 */
class XSCommandShowRulers : public XSCommand {
public:
    XSCommandShowRulers(bool showRulers);

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
    bool m_oldShowRulers;
    bool m_newShowRulers;
};


#endif
