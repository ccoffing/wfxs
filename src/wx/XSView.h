#ifndef _XS_VIEW_H
#define _XS_VIEW_H

/** @file
 *  @brief TODO
 */

#include <wx/docview.h>

/**
 *  @brief TODO
 */
class XSView : public wxView {
    DECLARE_DYNAMIC_CLASS(XSView)

public:
    XSView();
    virtual ~XSView();

    bool OnCreate(wxDocument* doc, long flags);
    void OnDraw(wxDC* dc);
    void OnUpdate(wxView* sender, wxObject* hint = (wxObject*)NULL);
    bool OnClose(bool deleteWindow = true);

    void OnCut(wxCommandEvent& event);
};


#endif
