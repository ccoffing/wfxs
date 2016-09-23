#include <wx/bmpbuttn.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/sizer.h>

#include "XSApplication.h"
#include "XSDocChild.h"
#include "XSMessages.h"
#include "XSMessages.h"
#include "XSToolWindow.h"
#include "resources/new.xpm"


BEGIN_EVENT_TABLE(XSToolWindow, wxDocParentFrame)
EVT_MENU(wxID_ABOUT, XSApplication::OnAbout)
EVT_BUTTON(XS_EVT_BEAD, XSToolWindow::OnButton)
EVT_BUTTON(XS_EVT_FULL_STITCH, XSToolWindow::OnButton)
EVT_BUTTON(XS_EVT_HALF_AUTO_STITCH, XSToolWindow::OnButton)
EVT_BUTTON(XS_EVT_QUARTER_AUTO_STITCH, XSToolWindow::OnButton)
EVT_BUTTON(XS_EVT_THREEQUARTER_AUTO_STITCH, XSToolWindow::OnButton)
END_EVENT_TABLE()

#if 0
wxBitmap * GetPNG() {
    wxBitmap *bmp = new wxBitmap(f, wxBITMAP_TYPE_PNG);

    return bmp;
}
#endif

XSToolWindow::XSToolWindow(wxDocManager* manager)
    : wxDocParentFrame(manager, NULL, wxID_ANY, wxT("Wildflower Cross Stitch"))
{
    wxMenuBar* menubar = new wxMenuBar();
    wxMenu* menu = new wxMenu();

    menu->Append(wxID_NEW, wxT("&New...\tCTRL-N"));
    menu->Append(wxID_OPEN, wxT("&Open...\tCTRL-O"));
    menu->Append(wxID_ANY, wxT("Open Recent"), new wxMenu());
    menu->Append(wxID_EXIT, wxT("Quit\tCTRL-Q"));
    menubar->Append(menu, wxT("File"));

    menu = new wxMenu();
    menu->Append(wxID_HELP, wxT("Help"));
    menu->Append(XS_EVT_TIP_OF_THE_DAY, wxT("Tip of the day"));
    menu->Append(wxID_ABOUT, wxT("About"));
    menubar->Append(menu, wxT("&Help"));

    SetMenuBar(menubar);

    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

    wxBitmap bmpBead(wxBITMAP(new));        // FIXME
    wxBitmap bmpFullStitch(wxBITMAP(new));  // FIXME

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);
    topsizer->Add(notebook);

    wxPanel* panel = new wxPanel(notebook);
    notebook->AddPage(panel, wxT("Stitches"));
    wxGridSizer* grid = new wxGridSizer(3, 3, 5, 5);
    grid->Add(new wxBitmapButton(panel, XS_EVT_BEAD, bmpBead));
    grid->Add(new wxBitmapButton(panel, XS_EVT_FULL_STITCH, bmpFullStitch));
    grid->Add(new wxBitmapButton(panel, XS_EVT_HALF_TOP_STITCH, bmpFullStitch));
    grid->Add(new wxBitmapButton(panel, XS_EVT_HALF_BOTTOM_STITCH, bmpFullStitch));
    panel->SetSizer(grid);

    panel = new wxPanel(notebook);
    notebook->AddPage(panel, wxT("Flosses"));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    Show();
}


void XSToolWindow::OnNew()
{
    GetDocumentManager()->CreateDocument(wxT(""), wxDOC_NEW);
}


void XSToolWindow::OnButton(wxCommandEvent& event)
{
    // FIXME
}


void XSToolWindow::HighlightTool(Tool tool)
{
    switch (tool) {
    case Tool_FullStitch:
        // FIXME
        break;
    case Tool_HalfAutoStitch:
    case Tool_HalfBottomStitch:
    case Tool_HalfTopStitch:
        // FIXME
        break;
    case Tool_ThreeQuarterAutoStitch:
    case Tool_ThreeQuarterULStitch:
    case Tool_ThreeQuarterURStitch:
    case Tool_ThreeQuarterLLStitch:
    case Tool_ThreeQuarterLRStitch:
        // FIXME
        break;
    case Tool_QuarterAutoStitch:
    case Tool_QuarterULStitch:
    case Tool_QuarterURStitch:
    case Tool_QuarterLLStitch:
    case Tool_QuarterLRStitch:
        // FIXME
        break;
    default:
        wxASSERT(0);
        break;
    }
}
