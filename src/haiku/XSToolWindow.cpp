#include "XSToolWindow.h"
#include "XSApplication.h"
#include "XSMessages.h"

#include <BAF/interface/LayoutAll.h>
#include <BAF/interface/Pos.h>
#include <app/Message.h>
#include <support/Debug.h>


XSToolWindow::XSToolWindow()
    : BAF::Window(BAF::Layout::PosOriginFrame(), "Tool Window", B_FLOATING_WINDOW_LOOK,
              B_FLOATING_APP_WINDOW_FEEL, B_WILL_ACCEPT_FIRST_CLICK | B_ASYNCHRONOUS_CONTROLS
                      | B_NOT_CLOSABLE | B_NOT_ZOOMABLE | B_NOT_MINIMIZABLE)
{
    BAF::TabView *view =
        new BAF::TabView(
                "T",
                new BAF::VGroup(B_ALIGN_LEFT,
#if 0
                        new BAF::Button("1", XS_EVT_FULL_STITCH),
                        new BAF::Button("2", XS_EVT_HALF_AUTO_STITCH),
                        0),
#else
                    new BAF::PictureButton("full", "full", new BMessage(XS_EVT_FULL_STITCH),
                                    B_TWO_STATE_BUTTON),
                    //                    new BAF::PictureButton(HalfOff, HalfOn,
                    //                        new BMessage(XS_EVT_FULL_STITCH), B_TWO_STATE_BUTTON),
                    0),
#endif
                "E",
                new BAF::VGroup(B_ALIGN_LEFT,
                        new BAF::Button("B", XS_EVT_BEAD),
                        0),
                0);

    AddChild(dynamic_cast<BView *>(view));

    ResizeToPreferred();
    Show();
}

XSToolWindow::~XSToolWindow()
{
}

bool XSToolWindow::QuitRequested()
{
    xs_app->PostMessage(B_QUIT_REQUESTED);
    return false;  // No, you can't close me (but the app might).
}

void XSToolWindow::MessageReceived(BMessage* message)
{
    switch (message->what) {
    case XS_EVT_HIGHLIGHT_TOOL: {
        Tool tool;
        status_t r = message->FindInt32("Tool", 0, (int32*)&tool);
        ASSERT(r == B_OK);
        HighlightTool(tool);
        break;
    }
    case XS_EVT_FULL_STITCH:
    case XS_EVT_HALF_AUTO_STITCH:
        // FIXME:  forward to window
        break;
    default:
        BAF::Window::MessageReceived(message);
    }
}

#if 0
bool XSToolWindow::ActivateControls()
{
    // m_menuBar.ActivateMenu(XSApplicationMenuUpdater());
    return true;
}

#endif


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
        ASSERT(0);
        break;
    }
}
