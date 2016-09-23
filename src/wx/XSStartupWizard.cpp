#include <wx/button.h>
#include <wx/sizer.h>

#include "XSStartupWizard.h"


BEGIN_EVENT_TABLE(XSStartupWizard, wxDialog)
END_EVENT_TABLE()

XSStartupWizard::XSStartupWizard(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("Welcome..."), wxDefaultPosition)
{
    wxSizer* sizerTop = new wxBoxSizer(wxVERTICAL);

    sizerTop->Add(new wxButton(this, wxID_NEW, wxT("New")), 0, wxCENTRE | wxALL, 5);

    SetSizer(sizerTop);
    sizerTop->Fit(this);
    CenterOnScreen();
    ShowModal();
}
