#ifndef _XS_WINDOW_H
#define _XS_WINDOW_H

#include "XSView.h"
#include <interface/Window.h>

namespace BAF {
class MenuBar;
}
class BMessage;
class XSDocument;
class XSFlossPaletteView;

class XSWindow : public BWindow {
public:
    XSWindow(unsigned int x, unsigned int y, XSDocument *document);
    virtual ~XSWindow();

    bool QuitRequested();
    virtual void MessageReceived(BMessage *message);
    virtual void WindowActivated(bool active);

    XSFlossPaletteView *GetFlossPaletteView();

    static BRect GetPreferredFrame(unsigned int x, unsigned int y);

    void UpdateMenu();

private:
    XSDocument *m_document;
    BAF::MenuBar *m_menuBar;
    XSView *m_view;
    XSFlossPaletteView *m_flossPaletteView;

    // Disallow copying
    XSWindow(XSWindow const &);
    XSWindow &operator=(XSWindow const &);
};

#endif
