#ifndef _XS_TOOL_WINDOW_H
#define _XS_TOOL_WINDOW_H

/** @file
 *  @brief TODO
 */

#include <wx/docview.h>

#include "XSToolTypes.h"

/**
 *  @brief TODO
 */
class XSToolWindow : public wxDocParentFrame {
//    DECLARE_CLASS(XSToolWindow)
    DECLARE_EVENT_TABLE()

public:
    XSToolWindow(wxDocManager *manager);  // const wxPoint &pos, const wxSize &size, long type);

    void OnButton(wxCommandEvent &event);
    void OnNew();

    void HighlightTool(Tool tool);
};

extern XSToolWindow *toolWindow;

#endif
