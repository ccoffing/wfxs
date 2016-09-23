#ifndef _XS_EDIT_FLOSS_WINDOW_H
#define _XS_EDIT_FLOSS_WINDOW_H

#include "XSFlossPalette.h"
#include <BAF/interface/Window.h>

class XSDocument;
class XSFlossPaletteView;
namespace BAF {
class PopUpMenuBar;
}

class XSEditFlossWindow : public BAF::Window {
public:
    XSEditFlossWindow(XSDocument* document, XSFlossPalette const& flossPalette);
    virtual ~XSEditFlossWindow();

    bool QuitRequested();

    virtual void MessageReceived(BMessage* message);

private:
    XSFlossPalette m_flossPalette;
    unsigned int m_flossIndex;
    XSFlossPaletteView* m_flossPaletteView;
    BAF::PopUpMenuBar* m_makerList;

    XSDocument* m_document;
    bool m_save;

    // Disallow copying
    XSEditFlossWindow(XSEditFlossWindow const&);
    XSEditFlossWindow& operator=(XSEditFlossWindow const&);
};

#endif
