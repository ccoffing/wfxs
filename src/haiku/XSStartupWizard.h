#ifndef _XS_STARTUP_WIZARD_H
#define _XS_STARTUP_WIZARD_H

#include <BAF/interface/Window.h>
class BMessage;

class XSStartupWizard : public BAF::Window {
public:
    XSStartupWizard();

    virtual void MessageReceived(BMessage *message);
};

#endif
