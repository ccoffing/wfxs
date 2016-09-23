#ifndef _XS_TOOL_WINDOW_H
#define _XS_TOOL_WINDOW_H

#include "XSToolTypes.h"
#include <BAF/interface/Window.h>
#include <interface/Window.h>

class XSToolWindow : public BAF::Window {
public:
    XSToolWindow();
    virtual ~XSToolWindow();

    void HighlightTool(Tool tool);

    virtual void MessageReceived(BMessage* message);
    virtual bool QuitRequested();
};

extern XSToolWindow* toolWindow;

#endif
