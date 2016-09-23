#include "XSWindow.h"
#include "XSApplication.h"
#include "XSDocument.h"
#include "XSFlossPaletteView.h"
#include "XSMessages.h"
#include "XSView.h"
#include "XSWindowMenuUpdater.h"

#include <app/Message.h>
#include <interface/MenuItem.h>
#include <interface/ScrollBar.h>
#include <interface/ScrollView.h>

#include <BAF/app/Messages.h>
#include <BAF/interface/Menu.h>
#include <BAF/interface/MenuBar.h>
#include <BAF/interface/MenuItem.h>
#include <BAF/interface/Pos.h>

class XSDocument;


XSWindow::XSWindow(unsigned int x, unsigned int y, XSDocument* document)
    : BWindow(XSWindow::GetPreferredFrame(x, y), NULL, B_DOCUMENT_WINDOW, 0)
    , m_document(document)
    , m_menuBar(new BAF::MenuBar(BRect(), "XSWindowMenuBar"))
    , m_view(new XSView(document, Bounds(), B_FOLLOW_ALL))
    , m_flossPaletteView(new XSFlossPaletteView(&document->GetToolState()->m_flossPalette,
              &document->GetToolState()->m_flossIndex, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM))
    , m_controller(document->getModel(), m_view, m_flossPaletteView)

{
    BAF::Menu *menu, *submenu, *subsubmenu;

    menu = new BAF::Menu("File");
    menu->AddItem(new BAF::MenuItem("New", BAF_MSG_NEW, 'N'));
    menu->AddItem(new BAF::MenuItem("Open", BAF_MSG_OPEN, 'O'));
    submenu = new BAF::Menu("Open Recent");
    menu->AddItem(submenu);
    menu->AddItem(new BAF::MenuItem("Import...", BAF_MSG_IMPORT));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Close", B_QUIT_REQUESTED, 'W'));
    menu->AddItem(new BAF::MenuItem("Save", BAF_MSG_SAVE, 'S'));
    menu->AddItem(new BAF::MenuItem("Save as...", BAF_MSG_SAVE_AS, 'S', B_SHIFT_KEY));
    menu->AddItem(new BAF::MenuItem("Revert to Saved", BAF_MSG_REVERT_TO_SAVED));
    menu->AddItem(new BAF::MenuItem("Export...", BAF_MSG_EXPORT));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Properties", BAF_MSG_PROPERTIES, '.'));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Print...", BAF_MSG_PRINT, 'P'));
    menu->AddItem(new BAF::MenuItem("Print Preview...", BAF_MSG_PRINT_PREVIEW, 'P', B_SHIFT_KEY));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Quit", BAF_MSG_QUIT_APP, 'Q'));
    m_menuBar->AddItem(menu);

    menu = new BAF::Menu("Edit");
    menu->AddItem(new BAF::MenuItem("Undo", BAF_MSG_UNDO, 'Z'));
    menu->AddItem(new BAF::MenuItem("Redo", BAF_MSG_REDO, 'Z', B_SHIFT_KEY));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Cut", BAF_MSG_CUT, 'X'));
    menu->AddItem(new BAF::MenuItem("Copy", BAF_MSG_COPY, 'C'));
    menu->AddItem(new BAF::MenuItem("Paste", BAF_MSG_PASTE, 'V'));
    menu->AddSeparatorItem();
    submenu = new BAF::Menu("Center");
    submenu->AddItem(new BAF::MenuItem("Horizontally", XS_EVT_H_CENTER));
    submenu->AddItem(new BAF::MenuItem("Vertically", XS_EVT_V_CENTER));
    submenu->AddItem(new BAF::MenuItem("Both", BAF_MSG_CENTER));
    menu->AddItem(submenu);
    m_menuBar->AddItem(menu);

    menu = new BAF::Menu("View");
    menu->AddItem(new BAF::MenuItem("Show Grid", XS_EVT_SHOW_GRID));
    menu->AddItem(new BAF::MenuItem("Show Rulers", XS_EVT_SHOW_RULERS));
    submenu = new BAF::Menu("Style");
    submenu->AddItem(new BAF::MenuItem("Pattern", XS_EVT_PATTERN_STYLE));
    submenu->AddItem(new BAF::MenuItem("Design", XS_EVT_DESIGN_STYLE));
    submenu->AddItem(new BAF::MenuItem("Realistic", XS_EVT_REALISTIC_STYLE));
    menu->AddItem(submenu);
    submenu = new BAF::Menu("Zoom");
    submenu->AddItem(new BAF::MenuItem("Zoom In", BAF_MSG_ZOOM_IN, '='));
    submenu->AddItem(new BAF::MenuItem("Zoom Out", BAF_MSG_ZOOM_OUT, '-'));
    menu->AddItem(submenu);
    m_menuBar->AddItem(menu);

    menu = new BAF::Menu("Tools");
    submenu = new BAF::Menu("Stitches");
    submenu->AddItem(new BAF::MenuItem("Full", XS_EVT_FULL_STITCH, '1'));
    subsubmenu = new BAF::Menu("Half");
    subsubmenu->AddItem(new BAF::MenuItem("Automatic", XS_EVT_HALF_AUTO_STITCH, '2'));
    subsubmenu->AddItem(new BAF::MenuItem("Top", XS_EVT_HALF_TOP_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Bottom", XS_EVT_HALF_BOTTOM_STITCH));
    submenu->AddItem(subsubmenu);
    subsubmenu = new BAF::Menu("Three-quarter");
    subsubmenu->AddItem(new BAF::MenuItem("Automatic", XS_EVT_THREEQUARTER_AUTO_STITCH, '3'));
    subsubmenu->AddItem(new BAF::MenuItem("Upper left", XS_EVT_THREEQUARTER_UL_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Upper right", XS_EVT_THREEQUARTER_UR_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Lower left", XS_EVT_THREEQUARTER_LL_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Lower right", XS_EVT_THREEQUARTER_LR_STITCH));
    submenu->AddItem(subsubmenu);
    subsubmenu = new BAF::Menu("Quarter");
    subsubmenu->AddItem(new BAF::MenuItem("Automatic", XS_EVT_QUARTER_AUTO_STITCH, '4'));
    subsubmenu->AddItem(new BAF::MenuItem("Upper left", XS_EVT_QUARTER_UL_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Upper right", XS_EVT_QUARTER_UR_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Lower left", XS_EVT_QUARTER_LL_STITCH));
    subsubmenu->AddItem(new BAF::MenuItem("Lower right", XS_EVT_QUARTER_LR_STITCH));
    submenu->AddItem(subsubmenu);
    menu->AddItem(submenu);
    menu->AddItem(new BAF::MenuItem("Beads", XS_EVT_BEAD));
    menu->AddItem(new BAF::MenuItem("French Knots", XS_EVT_FRENCH_KNOT));
    submenu = new BAF::Menu("Eraser");
    submenu->AddItem(new BAF::MenuItem("Small", XS_EVT_ERASER_SMALL));
    submenu->AddItem(new BAF::MenuItem("Medium", XS_EVT_ERASER_MEDIUM));
    submenu->AddItem(new BAF::MenuItem("Large", XS_EVT_ERASER_LARGE));
    menu->AddItem(submenu);
    menu->AddItem(new BAF::MenuItem("Flood Fill", XS_EVT_FLOOD_FILL));
    menu->AddItem(new BAF::MenuItem("Color Picker", XS_EVT_COLOR_PICKER));
    submenu = new BAF::Menu("Floss palette");
    submenu->AddItem(new BAF::MenuItem("Edit...", XS_EVT_EDIT_PALETTE));
    submenu->AddItem(new BAF::MenuItem("Clear unused entries", XS_EVT_CLEAR_UNUSED_PALETTE));
    menu->AddItem(submenu);
    submenu = new BAF::Menu("Layers");
    submenu->AddItem(new BAF::MenuItem("Add layer", XS_EVT_LAYER_ADD));
    submenu->AddItem(new BAF::MenuItem("Delete layer", XS_EVT_LAYER_DEL));
    submenu->AddItem(new BAF::MenuItem("Move layer up", XS_EVT_LAYER_UP));
    submenu->AddItem(new BAF::MenuItem("Move layer down", XS_EVT_LAYER_DOWN));
    menu->AddItem(submenu);
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("Overwrite", XS_EVT_OVERWRITE));
    m_menuBar->AddItem(menu);

    menu = new BAF::Menu("Windows");
    menu->AddItem(new BAF::MenuItem("Next", BAF_MSG_NEXT_WINDOW, '`'));
    menu->AddItem(new BAF::MenuItem("Previous", BAF_MSG_PREVIOUS_WINDOW));
    menu->AddSeparatorItem();
    // FIXME:  List of all windows
    m_menuBar->AddItem(menu);

    menu = new BAF::Menu("Help");
    menu->AddItem(new BAF::MenuItem("Help...", BAF_MSG_HELP));
    menu->AddItem(new BAF::MenuItem("Tip of the Day...", BAF_MSG_TIP_OF_THE_DAY));
    menu->AddSeparatorItem();
    menu->AddItem(new BAF::MenuItem("About...", B_ABOUT_REQUESTED));
    m_menuBar->AddItem(menu);

    AddChild(m_menuBar);

    float y = m_menuBar->Bounds().Height();
    m_view->MoveTo(0, y);
    m_view->ResizeBy(-B_V_SCROLL_BAR_WIDTH, -y - B_H_SCROLL_BAR_HEIGHT);
    BScrollView* scrollView
            = new BScrollView(NULL, m_view, m_view->ResizingMode(), B_WILL_DRAW, true, true);

    y += scrollView->Bounds().Height();
    m_flossPaletteView->MoveTo(0, y);
    MinMaxView mmv = m_flossPaletteView->layoutprefs();
    m_flossPaletteView->ResizeTo(m_view->Bounds().Width(), mmv.min.y);
    BScrollView* scrollFlossPaletteView = new BScrollView(
            NULL, m_flossPaletteView, m_flossPaletteView->ResizingMode(), B_WILL_DRAW, true, false);

    ResizeBy(0, scrollFlossPaletteView->Bounds().Height());

    AddChild(scrollView);
    AddChild(scrollFlossPaletteView);

    m_menuBar->UpdateAll(XSWindowMenuUpdater(m_document));
}

XSWindow::~XSWindow()
{
}

bool XSWindow::QuitRequested()
{
    BMessage m(BAF_MSG_CLOSE);

    m_document->MessageReceived(&m);
    if (m_document->IsClosable()) {
        delete m_document;
        return true;
    }
    return false;
}

void XSWindow::MessageReceived(BMessage* message)
{
    switch (message->what) {
    case BAF_MSG_QUIT_APP:
        xs_app->PostMessage(B_QUIT_REQUESTED);
        break;
    case B_ABOUT_REQUESTED:
        xs_app->PostMessage(B_ABOUT_REQUESTED);
        break;
    case B_SAVE_REQUESTED: {
        XSProperties properties = m_document->getModel().GetPropertiesCopy();

        entry_ref directory;
        if (message->FindRef("directory", 0, &directory) != 0)
            ASSERT(0);
        if (message->FindString("name", &properties.m_filename) != 0)
            ASSERT(0);

        m_document->getModel().SetProperties(properties);

        // FIXME:  pay attention to 'directory'
        Save(properties.m_filename);
        break;
    }
    case BAF_MSG_PROPERTIES: {
        if (m_propertiesWindow)
            m_propertiesWindow->Show();
        else
            m_propertiesWindow
                    = new XSPropertiesWindow(this, m_document->getModel().GetProperties());
        break;
    }

    case XS_EVT_PATTERN_STYLE:
        m_controller.OnPatternStyle();
        break;
    case XS_EVT_DESIGN_STYLE:
        m_controller.OnDesignStyle();
        break;
    case XS_EVT_REALISTIC_STYLE:
        m_controller.OnRealisticStyle();
        break;
    case BAF_MSG_DIALOG_CLOSING: {
        status_t s;
        int32_t dialog, button;

        s = message->FindInt32("dialog", &dialog);
        ASSERT(s == B_OK);
        s = message->FindInt32("button", &button);
        ASSERT(s == B_OK);

        switch ((XSWindowDialogs)dialog) {
        case XS_DIALOG_PROPERTIES:
            if (button == BAF_MSG_BUTTON_OKAY) {
                m_commandStack.Do(new XSCommandSetProperties(m_propertiesWindow->m_properties));
            }
            m_propertiesWindow->PostMessage(BAF_MSG_QUIT_REQUESTED);
            m_propertiesWindow = 0;
            break;
        case XS_DIALOG_EDIT_FLOSS:
            // FIXME -- retrieve settings
            m_editFlossWindow->PostMessage(BAF_MSG_QUIT_REQUESTED);
            m_editFlossWindow = 0;
            break;
        default:
            ASSERT(0);
        }
        break;
    }

    case BAF_MSG_CLOSE: {
        if (m_propertiesWindow) {
            m_propertiesWindow->PostMessage(BAF_MSG_QUIT_REQUESTED);
            m_propertiesWindow = 0;
        }
        if (m_commandStack.IsModified()) {
            BAlert* alert = new BAlert("title",
                    "Do you want to save changes to this pattern before closing it?  "
                    "If you don't save, your changes will be lost.",
                    "Cancel", "Don't save", "Save", B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
                    B_WARNING_ALERT);
            alert->SetShortcut(0, B_ESCAPE);
            int32 button_index = alert->Go();
            if (button_index == 2) {  // save
                Save();
                if (m_commandStack.IsModified()) {
                    printf("save apparently failed... not closing.\n");
                    break;
                }
            } else if (button_index == 0)  // cancel
                break;
            else   // don't save
                ;  // so fall through; clean up...
        }

        // FIXME:  send reply msg instead
        m_allowClose = true;
        break;
    }

    case BAF_MSG_SAVE:
        Save();
        break;

    case BAF_MSG_SAVE_AS:
        SaveAs();
        break;

    case BAF_MSG_REVERT_TO_SAVED: {
        BAlert* alert = new BAlert("Revert to saved?",
                "Do you want to discard your changes, and revert to the previously saved version?",
                "Cancel", "Discard & Revert", NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
                B_WARNING_ALERT);
        alert->SetShortcut(0, B_ESCAPE);
        int32 button_index = alert->Go();
        if (button_index == 1) {
            printf("FIXME:  revert\n");
        }
        break;
    }

    case BAF_MSG_UNDO:
        m_controller.Undo();
        break;
    case BAF_MSG_REDO:
        m_controller.Redo();
        break;
    case BAF_MSG_ZOOM_IN:
        m_controller.OnZoomIn();
        break;
    case BAF_MSG_ZOOM_OUT:
        m_controller.OnZoomOut();
        break;
    case XS_EVT_EDIT_PALETTE:
        if (m_editFlossWindow)
            m_editFlossWindow->Show();
        else
            m_editFlossWindow = new XSEditFlossWindow(this, ToolState().m_flossPalette);
        break;

    case BAF_MSG_OVERWRITE:
        ToolState().m_overwrite = !ToolState().m_overwrite;
        break;
    case XS_EVT_FULL_STITCH:
        m_controller.OnFullStitch();
        break;
    case XS_EVT_HALF_AUTO_STITCH:
        m_controller.OnHalfAutoStitch();
        break;
    case XS_EVT_HALF_TOP_STITCH:
        m_controller.OnHalfTopStitch();
        break;
    case XS_EVT_HALF_BOTTOM_STITCH:
        m_controller.OnHalfBottomStitch();
        break;
    case XS_EVT_THREEQUARTER_AUTO_STITCH:
        m_controller.OnThreeQuarterAutoStitch();
        break;
    case XS_EVT_THREEQUARTER_UL_STITCH:
        m_controller.OnThreeQuarterULStitch();
        break;
    case XS_EVT_THREEQUARTER_UR_STITCH:
        m_controller.OnThreeQuarterURStitch();
        break;
    case XS_EVT_THREEQUARTER_LL_STITCH:
        m_controller.OnThreeQuarterLLStitch();
        break;
    case XS_EVT_THREEQUARTER_LR_STITCH:
        m_controller.OnThreeQuarterLRStitch();
        break;
    case XS_EVT_QUARTER_AUTO_STITCH:
        m_controller.OnQuarterAutoStitch();
        break;
    case XS_EVT_QUARTER_UL_STITCH:
        m_controller.OnQuarterULStitch();
        break;
    case XS_EVT_QUARTER_UR_STITCH:
        m_controller.OnQuarterURStitch();
        break;
    case XS_EVT_QUARTER_LL_STITCH:
        m_controller.OnQuarterLLStitch();
        break;
    case XS_EVT_QUARTER_LR_STITCH:
        m_controller.OnQuarterLRStitch();
        break;
    case XS_EVT_LAYER_ADD:
        m_controller.OnLayerAdd();
        break;
    case XS_EVT_LAYER_DEL:
        m_controller.OnLayerDel();
        break;
    case XS_EVT_LAYER_UP:
        m_controller.OnLayerUp();
        break;
    case XS_EVT_LAYER_DOWN:
        m_controller.OnLayerDown();
        break;
    case XS_EVT_ERASER_SMALL:
        m_controller.OnToolEraserSmall();
        break;
    case XS_EVT_ERASER_MEDIUM:
        m_controller.OnToolEraserMedium();
        break;
    case XS_EVT_ERASER_LARGE:
        m_controller.OnToolEraserLarge();
        break;
    case XS_EVT_FLOOD_FILL:
        m_controller.OnToolFloodFill();
        break;
    case XS_EVT_COLOR_PICKER:
        m_controller.OnToolColorPicker();
        break;
    case BAF_MSG_IMPORT:
    case BAF_MSG_EXPORT:
    case BAF_MSG_PRINT:
    case BAF_MSG_PRINT_PREVIEW:
    case BAF_MSG_HELP:
    case BAF_MSG_TIP_OF_THE_DAY:
        printf("FIXME:  XSDocument: '%4s' not implemented yet\n", (char*)&message->what);
        break;

    default:
        return false;
    }
    m_window->UpdateMenu();
    return true;
}

XSFlossPaletteView* XSWindow::GetFlossPaletteView()
{
    return m_flossPaletteView;
}

// FIXME:  move this into mt-framework
BRect XSWindow::GetPreferredFrame(unsigned int x, unsigned int y)
{
    unsigned int zoom = XSView::GetPreferredZoom();
    BRect frame(0, 0, x * zoom, y * zoom);

    BRect prevFrame;

#if 0
    XSDocument *document = controller->GetActiveDocument();
    if (document)
        prevFrame = document->GetWindow()->Frame();
    else
#endif
    prevFrame = PosOriginFrame();

    frame = PosCascadeOff(frame, prevFrame, true);

    return frame;
}

void XSWindow::UpdateMenu()
{
    m_menuBar->UpdateAll(XSWindowMenuUpdater(m_document));
}

void XSWindow::WindowActivated(bool activated)
{
    if (activated) {
#if 0
        xs_app->DocumentActivated(m_document, Frame());
#endif
        m_menuBar->ActivateMenu();
        UpdateMenu();
        // FIXME
        // m_toolWindow->SetState(m_document->GetToolState());
    }
}
