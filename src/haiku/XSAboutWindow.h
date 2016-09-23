#ifndef _XS_ABOUT_WINDOW_H
#define _XS_ABOUT_WINDOW_H

#include <BAF/interface/Window.h>

class BMessage;

class XSAboutWindow : public BAF::Window {
public:
    XSAboutWindow();

    virtual void MessageReceived(BMessage* message);
};

#endif
