#ifndef XS_COMMAND_SHOW_GRID_H
#define XS_COMMAND_SHOW_GRID_H

/** @file
 *  @brief TODO
 */

#include "XSCommand.h"

/**
 *  @brief TODO
 */
class XSCommandShowGrid : public XSCommand {
public:
    XSCommandShowGrid(bool showGrid);

    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;

private:
    bool m_oldShowGrid;
    bool m_newShowGrid;
};


#endif
