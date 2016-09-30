#ifndef XS_COMMAND_ZOOM_H
#define XS_COMMAND_ZOOM_H

/** @file
 *  @brief TODO
 */

#include "XSCommand.h"

/**
 *  @brief TODO
 */
class XSCommandZoom : public XSCommand {
public:
    XSCommandZoom(unsigned int zoom);

    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;

private:
    unsigned int m_oldZoom;
    unsigned int m_newZoom;
};


#endif
