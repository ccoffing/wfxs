#ifndef _XS_WINDOW_MENU_UPDATER_H
#define _XS_WINDOW_MENU_UPDATER_H

#include <BAF/interface/MenuUpdater.h>

class XSController;
class BString;

class XSWindowMenuUpdater : public BAF::MenuUpdater {
public:
    XSWindowMenuUpdater(XSController*);

    virtual bool IsEnabled(uint32_t what) const;
    virtual bool IsMarked(uint32_t what) const;
    virtual bool GetLabel(uint32_t what, BString& label) const;

private:
    XSController* m_controller;
};


#endif
