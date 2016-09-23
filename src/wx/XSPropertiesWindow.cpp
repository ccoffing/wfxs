#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/statbox.h>

#include "XSApplication.h"
#include "XSDocChild.h"
#include "XSMessages.h"
#include "XSPropertiesWindow.h"

XSPropertiesWindow::XSPropertiesWindow(XSDocChild* parent, XSProperties const& properties)
    : wxFrame(parent, wxID_ANY, wxT("Properties"), wxPoint(100, 100), wxSize(100, 100))
    ,
    // wxDefaultPosition, wxDefaultSize),
    m_properties(properties)
    , m_document(parent)
    , m_save(false)
{
    // wxPanel *panel = new wxPanel( this, wxID_ANY );
    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

#if 0
    topsizer->Add(new wxStaticBox(this, wxID_ANY, wxT("stuff")));
    wxNotebook *notebook = new wxNotebook(this, wxID_ANY);
    wxStaticBox *sbox;
    notebook->AddPage(new wxStaticBox(this, wxID_ANY, wxT("y")), wxT("Pattern Size"));
//    notebook->AddPage(sbox = new wxStaticBox(this, wxID_ANY, wxT("z")), wxT("Description"));
    wxBoxSizer *descsizer = new wxBoxSizer(wxVERTICAL);
    notebook->AddPage(descsizer, wxT("Description"));
    xxxxxxxx
    descsizer->Add(sbox);
    descsizer->Add((m_title = new wxTextCtrl()));
    descsizer->Add((m_author = new wxTextCtrl()));
    descsizer->Add((m_fabric = new wxTextCtrl()));
    notebook->AddPage(new wxTextCtrl(), wxT("Instructions"));
    topsizer->Add(notebook);
#else
    topsizer->Add((m_title = new wxTextCtrl(this, -1, wxT("foo"))), wxALIGN_LEFT);
    topsizer->Add((m_author = new wxTextCtrl(this, -1, wxT("bar"))), wxALIGN_LEFT);
    topsizer->Add((m_fabric = new wxTextCtrl(this, -1, wxT("baz"))), wxALIGN_LEFT);
#endif
    // panel->SetSizer(topsizer);
    SetSizer(topsizer);

#if 0
    new HGroup(B_ALIGN_MIDDLE,
            new HSpacer(),
            ok_button = new Button("OK", wxID_OK),
            new Button("Cancel", B_QUIT_REQUESTED),
#endif

    SetProperties();

    CentreOnParent();
    Show();
}


XSPropertiesWindow::~XSPropertiesWindow()
{
}


void XSPropertiesWindow::GetProperties()
{
    m_properties.m_title = m_title->GetValue();
    m_properties.m_author = m_author->GetValue();
    m_properties.m_fabric = m_fabric->GetValue();
    m_properties.m_instructions = m_instructions->GetValue();
}


void XSPropertiesWindow::SetProperties()
{
    m_title->ChangeValue(m_properties.m_title);
    m_author->ChangeValue(m_properties.m_author);
    m_fabric->ChangeValue(m_properties.m_fabric);
}
