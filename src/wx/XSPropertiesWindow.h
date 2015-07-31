#ifndef _XS_PROPERTIES_WINDOW_H
#define _XS_PROPERTIES_WINDOW_H

/** @file
 *  @brief TODO
 */

#include <wx/frame.h>
#include <wx/textctrl.h>

#include "XSProperties.h"

class XSDocChild;

/**
 *  @brief TODO
 */
class XSPropertiesWindow : public wxFrame {
public:
    XSPropertiesWindow(XSDocChild *parent,
            XSProperties const &properties);
    virtual ~XSPropertiesWindow();

    XSProperties m_properties;

protected:
    void GetProperties();
    void SetProperties();

private:
    XSDocChild *m_document;
    bool m_save;

    wxTextCtrl *m_author;
    wxTextCtrl *m_fabric;
    wxTextCtrl *m_title;
    wxTextCtrl *m_instructions;
};

#endif
