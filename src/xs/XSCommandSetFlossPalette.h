#ifndef XS_COMMAND_SET_FLOSS_PALETTE_H
#define XS_COMMAND_SET_FLOSS_PALETTE_H

#include "XSCommand.h"
#include "XSCommandSetFlossPalette.h"
#include "XSFlossPalette.h"

/**
 */
class XSCommandSetFlossPalette : public XSCommand {
public:
    XSCommandSetFlossPalette(XSFlossPalette const& flossPalette);

    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;

private:
    XSFlossPalette m_oldFlossPalette;
    XSFlossPalette m_newFlossPalette;

    // Disallow copying
    XSCommandSetFlossPalette(XSCommandSetFlossPalette const&);
    XSCommandSetFlossPalette& operator=(XSCommandSetFlossPalette const&);
};

#endif
