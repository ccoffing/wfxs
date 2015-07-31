#ifndef _XS_EDIT_FLOSS_WINDOW_H
#define _XS_EDIT_FLOSS_WINDOW_H

/** @file
 *  @brief TODO
 */

#include <wx/frame.h>

#include "XSFlossPalette.h"

class XSDocChild;
class XSFlossPaletteView;

/**
 *  @brief TODO
 */
class XSEditFlossWindow : public wxFrame {
public:
    XSEditFlossWindow(XSDocChild *document,
            XSFlossPalette const &flossPalette);
    virtual ~XSEditFlossWindow();

private:
    XSFlossPalette m_flossPalette;
    unsigned int m_flossIndex;
    XSFlossPaletteView *m_flossPaletteView;
//    xxx * m_makerList;

    XSDocChild *m_document;

    // Disallow copying
    XSEditFlossWindow(XSEditFlossWindow const &);
    XSEditFlossWindow &operator=(XSEditFlossWindow const &);

    DECLARE_EVENT_TABLE()
};

#endif
