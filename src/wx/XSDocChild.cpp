#include <wx/defs.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>

#include "Exceptions.h"
#include "XSApplication.h"
#include "XSCanvas.h"
#include "XSDocChild.h"
#include "XSEditFlossWindow.h"
#include "XSFileNative.h"
#include "XSFlossPalette.h"
#include "XSFlossPaletteView.h"
#include "XSMessages.h"
#include "XSPropertiesWindow.h"
#include "XSToolWindow.h"
#include "XSView.h"


BEGIN_EVENT_TABLE(XSDocChild, wxDocChildFrame)
EVT_MENU(wxID_NEW, XSDocChild::OnNew)
EVT_MENU(wxID_ABOUT, XSApplication::OnAbout)
EVT_MENU(wxID_PROPERTIES, XSDocChild::OnProperties)
EVT_MENU(wxID_ZOOM_IN, XSDocChild::OnZoomIn)
EVT_MENU(wxID_ZOOM_OUT, XSDocChild::OnZoomOut)
EVT_MENU(XS_EVT_COLOR_PICKER, XSDocChild::OnColorPicker)
EVT_MENU(XS_EVT_IMPORT, XSDocChild::OnImport)
EVT_MENU(XS_EVT_EXPORT, XSDocChild::OnExport)
EVT_MENU(XS_EVT_DESIGN_STYLE, XSDocChild::OnDesignStyle)
EVT_MENU(XS_EVT_ERASER_LARGE, XSDocChild::OnToolEraserLarge)
EVT_MENU(XS_EVT_ERASER_MEDIUM, XSDocChild::OnToolEraserMedium)
EVT_MENU(XS_EVT_ERASER_SMALL, XSDocChild::OnToolEraserSmall)
EVT_MENU(XS_EVT_FLOOD_FILL, XSDocChild::OnToolFloodFill)
EVT_MENU(XS_EVT_FULL_STITCH, XSDocChild::OnFullStitch)
EVT_MENU(XS_EVT_HALF_AUTO_STITCH, XSDocChild::OnHalfAutoStitch)
EVT_MENU(XS_EVT_HALF_BOTTOM_STITCH, XSDocChild::OnHalfBottomStitch)
EVT_MENU(XS_EVT_HALF_TOP_STITCH, XSDocChild::OnHalfTopStitch)
EVT_MENU(XS_EVT_LAYER_ADD, XSDocChild::OnLayerAdd)
EVT_MENU(XS_EVT_LAYER_DEL, XSDocChild::OnLayerDel)
EVT_MENU(XS_EVT_LAYER_DOWN, XSDocChild::OnLayerDown)
EVT_MENU(XS_EVT_LAYER_UP, XSDocChild::OnLayerUp)
EVT_MENU(XS_EVT_OVERWRITE, XSDocChild::OnOverwrite)
EVT_MENU(XS_EVT_PATTERN_STYLE, XSDocChild::OnPatternStyle)
EVT_MENU(XS_EVT_QUARTER_AUTO_STITCH, XSDocChild::OnQuarterAutoStitch)
EVT_MENU(XS_EVT_QUARTER_LL_STITCH, XSDocChild::OnQuarterLLStitch)
EVT_MENU(XS_EVT_QUARTER_LR_STITCH, XSDocChild::OnQuarterLRStitch)
EVT_MENU(XS_EVT_QUARTER_UL_STITCH, XSDocChild::OnQuarterULStitch)
EVT_MENU(XS_EVT_QUARTER_UR_STITCH, XSDocChild::OnQuarterURStitch)
EVT_MENU(XS_EVT_REALISTIC_STYLE, XSDocChild::OnRealisticStyle)
EVT_MENU(XS_EVT_SHOW_GRID, XSDocChild::OnShowGrid)
EVT_MENU(XS_EVT_THREEQUARTER_AUTO_STITCH, XSDocChild::OnThreeQuarterAutoStitch)
EVT_MENU(XS_EVT_THREEQUARTER_LL_STITCH, XSDocChild::OnThreeQuarterLLStitch)
EVT_MENU(XS_EVT_THREEQUARTER_LR_STITCH, XSDocChild::OnThreeQuarterLRStitch)
EVT_MENU(XS_EVT_THREEQUARTER_UL_STITCH, XSDocChild::OnThreeQuarterULStitch)
EVT_MENU(XS_EVT_THREEQUARTER_UR_STITCH, XSDocChild::OnThreeQuarterURStitch)
EVT_CLOSE(XSDocChild::OnCloseWindow)
EVT_MENU(XS_EVT_EDIT_PALETTE, XSDocChild::OnEditFlossPalette)
//    EVT_MENU(XS_EVT_REVERT_TO_SAVED, XSDocChild::OnRevertToSaved)
END_EVENT_TABLE()


XSDocChild::XSDocChild(XSDocument* doc, XSView* view)
    : wxDocChildFrame(doc, view, toolWindow, wxID_ANY, wxT("Untitled"))
    , m_document(doc)
    , m_view(new XSCanvasPage(this, this))
    , m_menuBar(new wxMenuBar())
    , m_flossPaletteView(new XSFlossPaletteView(
              &m_document->m_toolState.m_flossPalette, &m_document->m_toolState.m_flossIndex))
    , m_propertiesWindow(0)
    , m_editFlossWindow(0)
{
    m_document->SetDocChild(this);
    m_document->Resize(50, 50);

    wxMenu *menu, *submenu, *subsubmenu;

    menu = new wxMenu();
    menu->Append(wxID_NEW, wxT("&New...\tCTRL-N"));
    menu->Append(wxID_OPEN, wxT("&Open...\tCTRL-O"));
    menu->Append(wxID_ANY, wxT("Open Recent"), new wxMenu());
    menu->Append(XS_EVT_IMPORT, wxT("Import..."));
    menu->AppendSeparator();
    menu->Append(wxID_CLOSE, wxT("&Close\tCTRL-W"));
    menu->Append(wxID_SAVE, wxT("&Save\tCTRL-S"));
    menu->Append(wxID_SAVEAS, wxT("Save &As..."));
    menu->Append(wxID_REVERT_TO_SAVED, wxT("Revert to Saved"));
    menu->Append(XS_EVT_EXPORT, wxT("Export..."));
    menu->AppendSeparator();
    menu->Append(wxID_PROPERTIES, wxT("Properties"));
    menu->AppendSeparator();
    menu->Append(wxID_PRINT, wxT("Print"));
    menu->Append(wxID_PREVIEW, wxT("Print Preview"));
    menu->AppendSeparator();
    menu->Append(wxID_EXIT, wxT("Quit\tCTRL-Q"));
    m_menuBar->Append(menu, wxT("File"));

    menu = new wxMenu();
    menu->Append(wxID_UNDO, wxT("&Undo\tCTRL-Z"));
    menu->Append(wxID_REDO, wxT("&Redo\tCTRL-SHIFT-Z"));
    menu->AppendSeparator();
    menu->Append(wxID_CUT, wxT("Cut\tCTRL-X"));
    menu->Append(wxID_COPY, wxT("Copy\tCTRL-C"));
    menu->Append(wxID_PASTE, wxT("Paste\tCTRL-V"));
    menu->AppendSeparator();
    submenu = new wxMenu();
    submenu->Append(XS_EVT_H_CENTER, wxT("Horizontally"));
    submenu->Append(XS_EVT_V_CENTER, wxT("Vertically"));
    submenu->Append(XS_EVT_CENTER, wxT("Both"));
    menu->Append(wxID_ANY, wxT("Center"), submenu);
    m_menuBar->Append(menu, wxT("Edit"));

    menu = new wxMenu();
    menu->Append(XS_EVT_SHOW_GRID, wxT("Show Grid"));
    menu->Append(XS_EVT_SHOW_RULERS, wxT("Show Rulers"));
    submenu = new wxMenu();
    submenu->Append(XS_EVT_PATTERN_STYLE, wxT("Pattern"));
    submenu->Append(XS_EVT_DESIGN_STYLE, wxT("Design"));
    submenu->Append(XS_EVT_REALISTIC_STYLE, wxT("Realistic"));
    menu->Append(wxID_ANY, wxT("Style"), submenu);
    submenu = new wxMenu();
    submenu->Append(wxID_ZOOM_IN, wxT("Zoom In\tCTRL-+"));
    submenu->Append(wxID_ZOOM_OUT, wxT("Zoom Out\tCTRL--"));
    menu->Append(wxID_ANY, wxT("Zoom"), submenu);
    m_menuBar->Append(menu, wxT("View"));

    menu = new wxMenu();
    submenu = new wxMenu();
    submenu->Append(XS_EVT_FULL_STITCH, wxT("Full\tCTRL-1"));
    subsubmenu = new wxMenu();
    subsubmenu->Append(XS_EVT_HALF_AUTO_STITCH, wxT("Automatic\tCTRL-2"));
    subsubmenu->Append(XS_EVT_HALF_TOP_STITCH, wxT("Top"));
    subsubmenu->Append(XS_EVT_HALF_BOTTOM_STITCH, wxT("Bottom"));
    submenu->Append(wxID_ANY, wxT("Half"), subsubmenu);
    subsubmenu = new wxMenu();
    subsubmenu->Append(XS_EVT_THREEQUARTER_AUTO_STITCH, wxT("Automatic\tCTRL-3"));
    subsubmenu->Append(XS_EVT_THREEQUARTER_UL_STITCH, wxT("Upper left"));
    subsubmenu->Append(XS_EVT_THREEQUARTER_UR_STITCH, wxT("Upper right"));
    subsubmenu->Append(XS_EVT_THREEQUARTER_LL_STITCH, wxT("Lower left"));
    subsubmenu->Append(XS_EVT_THREEQUARTER_LR_STITCH, wxT("Lower right"));
    submenu->Append(wxID_ANY, wxT("Three-quarter"), subsubmenu);
    subsubmenu = new wxMenu();
    subsubmenu->Append(XS_EVT_QUARTER_AUTO_STITCH, wxT("Automatic\tCTRL-4"));
    subsubmenu->Append(XS_EVT_QUARTER_UL_STITCH, wxT("Upper left"));
    subsubmenu->Append(XS_EVT_QUARTER_UR_STITCH, wxT("Upper right"));
    subsubmenu->Append(XS_EVT_QUARTER_LL_STITCH, wxT("Lower left"));
    subsubmenu->Append(XS_EVT_QUARTER_LR_STITCH, wxT("Lower right"));
    submenu->Append(wxID_ANY, wxT("Quarter"), subsubmenu);
    menu->Append(wxID_ANY, wxT("Stitches\tCTRL-ALT-S"), submenu);
    menu->Append(XS_EVT_BEAD, wxT("Beads\tCTRL-ALT-B"));
    menu->Append(XS_EVT_FRENCH_KNOT, wxT("French Knots"));
    submenu = new wxMenu();
    submenu->Append(XS_EVT_ERASER_SMALL, wxT("Small"));
    submenu->Append(XS_EVT_ERASER_MEDIUM, wxT("Medium"));
    submenu->Append(XS_EVT_ERASER_LARGE, wxT("Large"));
    menu->Append(wxID_ANY, wxT("Eraser\tCTRL-ALT-E"), submenu);
    menu->Append(XS_EVT_FLOOD_FILL, wxT("Flood Fill\tCTRL-ALT-F"));
    menu->Append(XS_EVT_COLOR_PICKER, wxT("Color Picker\tCTRL-ALT-C"));
    submenu = new wxMenu();
    submenu->Append(XS_EVT_EDIT_PALETTE, wxT("Edit..."));
    submenu->Append(XS_EVT_CLEAR_UNUSED_PALETTE, wxT("Clear unused entries"));
    menu->Append(wxID_ANY, wxT("Floss palette"), submenu);
    submenu = new wxMenu();
    submenu->Append(XS_EVT_LAYER_ADD, wxT("Add layer"));
    submenu->Append(XS_EVT_LAYER_DEL, wxT("Delete layer"));
    submenu->Append(XS_EVT_LAYER_UP, wxT("Move layer up\tCTRL-ALT-UP"));
    submenu->Append(XS_EVT_LAYER_DOWN, wxT("Move layer down\tCTRL-ALT-DOWN"));
    menu->Append(wxID_ANY, wxT("Layers"), submenu);
    menu->AppendSeparator();
    menu->Append(XS_EVT_OVERWRITE, wxT("Overwrite"));
    m_menuBar->Append(menu, wxT("Tools"));

    menu = new wxMenu();
    menu->Append(wxID_HELP, wxT("Help"));
    menu->Append(XS_EVT_TIP_OF_THE_DAY, wxT("Tip of the day"));
    menu->AppendSeparator();
    menu->Append(wxID_ABOUT, wxT("About"));
    m_menuBar->Append(menu, wxT("&Help"));

    SetMenuBar(m_menuBar);
    CreateStatusBar();
    Show(true);
}


void XSDocChild::HighlightTool(Tool tool)
{
    // BMessage m(XS_EVT_HIGHLIGHT_TOOL);
    // m.AddInt32("Tool", tool);
    // m_toolWindow->PostMessage(&m);
}


void XSDocChild::OnNew(wxCommandEvent& WXUNUSED(event))
{
    toolWindow->OnNew();
}


void XSDocChild::OnImport(wxCommandEvent& WXUNUSED(event))
{
    // FIXME
}


void XSDocChild::OnExport(wxCommandEvent& WXUNUSED(event))
{
    // FIXME
}


void XSDocChild::OnProperties(wxCommandEvent& WXUNUSED(event))
{
    if (m_propertiesWindow)
        m_propertiesWindow->Show();
    else
        m_propertiesWindow = new XSPropertiesWindow(this, m_document->GetProperties());
}


void XSDocChild::OnEditFlossPalette(wxCommandEvent& WXUNUSED(event))
{
    m_editFlossWindow = new XSEditFlossWindow(this, m_document->ToolState().m_flossPalette);
}


void XSDocChild::OnPatternStyle(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitDrawStyle(DrawStyle_Pattern);
}


void XSDocChild::OnDesignStyle(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitDrawStyle(DrawStyle_Design);
}


void XSDocChild::OnRealisticStyle(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitDrawStyle(DrawStyle_Realistic);
}


void XSDocChild::OnShowGrid(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitToggleShowGrid();
}


#if 0
void x()
{
    switch (0) {

    case XS_EVT_DIALOG_CLOSING:
    {
        int s;
        int32_t dialog, button;

        s = message->FindInt32("dialog", &dialog);
        wxASSERT(s == B_OK);
        s = message->FindInt32("button", &button);
        wxASSERT(s == B_OK);

        switch ((XSWindowDialogs)dialog) {
        case XS_DIALOG_PROPERTIES:
            if ((unsigned int)button == XS_EVT_BUTTON_OKAY) {
                m_commandStack.Do(new XSCommandSetProperties(m_propertiesWindow->m_properties));
            }
            m_propertiesWindow->PostMessage(XS_EVT_QUIT_REQUESTED);
            m_propertiesWindow = 0;
            break;
        case XS_DIALOG_EDIT_FLOSS:
            // FIXME -- retrieve settings
            m_editFlossWindow->PostMessage(XS_EVT_QUIT_REQUESTED);
            m_editFlossWindow = 0;
            break;
        default:
            wxASSERT(0);
        }
        break;
    }

    case XS_EVT_REVERT_TO_SAVED:
    {
        BAlert *alert = new BAlert(wxT("Revert to saved?"),
                wxT("Do you want to discard your changes, and revert to the previously saved version?"),
                wxT("Cancel"), wxT("Discard & Revert"), NULL,
                B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT);
        alert->SetShortcut(0, B_ESCAPE);
        int32 button_index = alert->Go();
        if (button_index == 1) {
            printf("FIXME:  revert\n");
        }
        break;
    }

    case XS_EVT_EDIT_PALETTE:
        if (m_editFlossWindow)
            m_editFlossWindow->Show();
        else
            m_editFlossWindow = new XSEditFlossWindow(this, m_document->ToolState().m_flossPalette);
        break;
    }
}
#endif


void XSDocChild::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
    unsigned int zoom = m_document->GetZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom += increment;
    m_document->SubmitSetZoom(zoom);
}

void XSDocChild::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
    unsigned int zoom = m_document->GetZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom -= increment;
    m_document->SubmitSetZoom(zoom);
}

void XSDocChild::OnOverwrite(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_overwrite = !m_document->ToolState().m_overwrite;
}

void XSDocChild::OnFullStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_Full;
    HighlightTool(Tool_FullStitch);
}

void XSDocChild::OnHalfAutoStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_HalfAuto;
    HighlightTool(Tool_HalfAutoStitch);
}

void XSDocChild::OnHalfTopStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_HalfTop;
    HighlightTool(Tool_HalfTopStitch);
}

void XSDocChild::OnHalfBottomStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_HalfBottom;
    HighlightTool(Tool_HalfBottomStitch);
}

void XSDocChild::OnThreeQuarterAutoStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_ThreeQuarterAuto;
    HighlightTool(Tool_ThreeQuarterAutoStitch);
}

void XSDocChild::OnThreeQuarterULStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_ThreeQuarterUL;
    HighlightTool(Tool_ThreeQuarterULStitch);
}

void XSDocChild::OnThreeQuarterURStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_ThreeQuarterUR;
    HighlightTool(Tool_ThreeQuarterURStitch);
}

void XSDocChild::OnThreeQuarterLLStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_ThreeQuarterLL;
    HighlightTool(Tool_ThreeQuarterLLStitch);
}

void XSDocChild::OnThreeQuarterLRStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_ThreeQuarterLR;
    HighlightTool(Tool_ThreeQuarterLRStitch);
}

void XSDocChild::OnQuarterAutoStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_QuarterAuto;
    HighlightTool(Tool_QuarterAutoStitch);
}

void XSDocChild::OnQuarterULStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_QuarterUL;
    HighlightTool(Tool_QuarterULStitch);
}

void XSDocChild::OnQuarterURStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_QuarterUR;
    HighlightTool(Tool_QuarterURStitch);
}

void XSDocChild::OnQuarterLLStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_QuarterLL;
    HighlightTool(Tool_QuarterLLStitch);
}

void XSDocChild::OnQuarterLRStitch(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Stitch;
    m_document->ToolState().m_stitchType = Stitch_QuarterLR;
    HighlightTool(Tool_QuarterLRStitch);
}

void XSDocChild::OnLayerAdd(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitLayerAdd();
}

void XSDocChild::OnLayerDel(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitLayerDel();
}

void XSDocChild::OnLayerUp(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitLayerUp();
}

void XSDocChild::OnLayerDown(wxCommandEvent& WXUNUSED(event))
{
    m_document->SubmitLayerDown();
}

void XSDocChild::OnToolEraserSmall(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Eraser;
}

void XSDocChild::OnToolEraserMedium(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Eraser;
}

void XSDocChild::OnToolEraserLarge(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_Eraser;
}

void XSDocChild::OnToolFloodFill(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_FloodFill;
}

void XSDocChild::OnColorPicker(wxCommandEvent& WXUNUSED(event))
{
    m_document->ToolState().m_toolType = ToolType_ColorPicker;
}


#if 0
void XSDocChild::SetTool(Tool tool)
{
    switch (tool) {
    case Tool_FullStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_Full;
        break;
    case Tool_ThreeQuarterAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterAuto;
        break;
    case Tool_ThreeQuarterULStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterUL;
        break;
    case Tool_ThreeQuarterURStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterUR;
        break;
    case Tool_ThreeQuarterLLStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterLL;
        break;
    case Tool_ThreeQuarterLRStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterLR;
        break;
    case Tool_HalfAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfAuto;
        break;
    case Tool_HalfBottomStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfBottom;
        break;
    case Tool_HalfTopStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfTop;
        break;
    case Tool_QuarterAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterAuto;
        break;
    case Tool_QuarterULStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterUL;
        break;
    case Tool_QuarterURStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterUR;
        break;
    case Tool_QuarterLLStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterLL;
        break;
    case Tool_QuarterLRStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterLR;
        break;
    case Tool_SpecialStitch:
        m_toolType = ToolType_Stitch;
        // FIXME
        break;
    case Tool_FrenchKnot:
        m_toolType = ToolType_Knot;
        // FIXME
        break;
    case Tool_LazyDaisyKnot:
        m_toolType = ToolType_Knot;
        // FIXME
        break;
    case Tool_Bead:
        m_toolType = ToolType_Bead;
        break;
    case Tool_Backstitch:
        m_toolType = ToolType_Backstitch;
        break;
    case Tool_Couching:
        m_toolType = ToolType_Couching;
        break;
    case Tool_RectangularSelect:
        m_toolType = ToolType_Select;
        // FIXME
        break;
    case Tool_FreeFormSelect:
        m_toolType = ToolType_Select;
        // FIXME
        break;
    default:
        wxASSERT(0);
    }
}
#endif


void XSDocChild::PropertiesWindowClosing(XSProperties const& properties)
{
    m_propertiesWindow = 0;
    m_document->SubmitProperties(properties);
}


void XSDocChild::FlossPaletteWindowClosing(XSFlossPalette const& flossPalette)
{
    m_editFlossWindow = 0;
    m_document->SubmitFlossPalette(flossPalette);
}


void XSDocChild::OnClearSquare(unsigned int x, unsigned int y)
{
    m_document->SubmitClearSquare(x, y, m_document->GetCurrentLayerIndex());
}


void XSDocChild::OnClearSquares(std::vector<wxPoint> const& points)
{
    m_document->SubmitClearSquares(points, m_document->GetCurrentLayerIndex());
}


void XSDocChild::OnSetKnot(unsigned int x, unsigned int y, unsigned int region)
{
    m_document->SubmitSetKnot(x, y, region, m_document->ToolState().m_knotType,
            m_document->ToolState().m_flossIndex, m_document->ToolState().m_overwrite);
}


void XSDocChild::OnSetStitches(std::vector<wxPoint>& points)
{
    m_document->SubmitSetStitches(points, m_document->ToolState().m_stitchTypeContinued,
            m_document->ToolState().m_flossIndex, m_document->ToolState().m_overwrite);
}


void XSDocChild::OnSetStitch(
        unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent)
{
    m_document->SubmitSetStitch(x, y, xPercent, yPercent, m_document->ToolState().m_stitchType,
            m_document->ToolState().m_flossIndex, m_document->ToolState().m_overwrite);
    m_document->ToolState().m_stitchTypeContinued = m_document->ToolState().m_stitchType;
}


void XSDocChild::OnFloodFill(
        unsigned int x, unsigned int y, XSSquareIO const& oldSquare, XSSquareIO const& newSquare)
{
    m_document->SubmitFloodFill(x, y, oldSquare, newSquare);
}


void XSDocChild::OnSetBead(unsigned int x, unsigned int y, unsigned int region)
{
    m_document->SubmitSetBead(
            x, y, region, m_document->ToolState().m_beadIndex, m_document->ToolState().m_overwrite);
}


StitchType XSDocChild::GetStitchType() const
{
    return m_document->m_toolState.m_stitchType;
}


ToolType XSDocChild::GetToolType() const
{
    return m_document->m_toolState.m_toolType;
}


void XSDocChild::OnCloseWindow(wxCloseEvent& event)
{
    bool can_close = false;

    if (m_propertiesWindow) {
        // FIXME  ... save these?  Or discard?
        // m_propertiesWindow->GetProperties();
        // FIXME  close properties window
    }
    if (m_document->IsModified()) {
        wxMessageDialog* dialog = new wxMessageDialog(this,
                wxT("Do you want to save changes to this pattern before closing it?  "
                    "If you don't save, your changes will be lost."),
                wxT("My app"), wxYES_NO | wxCANCEL);
        int ans = dialog->ShowModal();
        dialog->Destroy();
        switch (ans) {
        case wxID_YES:
            m_document->Save();
            if (m_document->IsModified())
                printf("save apparently failed... not closing.\n");
            else
                Destroy();
            break;
        case wxID_NO:
            Destroy();
            break;
        case wxID_CANCEL:
        default:
            if (!event.CanVeto())
                Destroy();
            else
                event.Veto();
            break;
        }
    } else
        Destroy();
}


#if 0
// FIXME:  move this into mt-framework
wxRect XSDocChild::GetPreferredFrame(unsigned int x,
        unsigned int y)
{
    unsigned int zoom = XSCanvas::GetPreferredZoom();
    wxRect frame(0, 0, x * zoom, y * zoom);

    wxRect prevFrame;
    XSDocChild *document = xs_app->m_manager.GetCurrentDocument();

    if (document)
        prevFrame = document->Frame();
    else
        prevFrame = PosOriginFrame();

    frame = PosCascadeOff(frame, prevFrame, true);

    return frame;
}
#endif


void XSDocChild::OnKeyDown(wxKeyEvent& event)
{
    unsigned int zoom = m_document->GetZoom();

    switch (event.GetKeyCode()) {
    case 'A':
    case 'a':
        GetCanvas()->ScrollBy(zoom, 0);
        break;
    case 'S':
    case 's':
        GetCanvas()->ScrollBy(0, -zoom);
        break;
    case 'W':
    case 'w':
        GetCanvas()->ScrollBy(0, zoom);
        break;
    case 'D':
    case 'd':
        GetCanvas()->ScrollBy(-zoom, 0);
        break;
    case WXK_LEFT:
        GetCanvas()->MoveCursorBy(-1, 0);
        break;
    case WXK_RIGHT:
        GetCanvas()->MoveCursorBy(1, 0);
        break;
    case WXK_UP:
        GetCanvas()->MoveCursorBy(0, -1);
        break;
    case WXK_DOWN:
        GetCanvas()->MoveCursorBy(0, 1);
        break;
    case ' ': {
        GetCanvas()->KeyboardClick();
        break;
    }
    case '[':
        m_document->PreviousFloss();
        break;
    case ']':
        m_document->NextFloss();
        break;
    }
}
