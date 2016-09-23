#include <BAF/app/Messages.h>
#include <BAF/interface/LayoutAll.h>
#include <BAF/interface/Pos.h>
#include <interface/MenuItem.h>

#include "XSData.h"
#include "XSDocument.h"
#include "XSEditFlossWindow.h"
#include "XSFlossPaletteView.h"
#include "XSMessages.h"


XSEditFlossWindow::XSEditFlossWindow(XSDocument* document, XSFlossPalette const& flossPalette)
    : BAF::Window(BRect(), "Floss Palette", B_TITLED_WINDOW_LOOK, B_MODAL_SUBSET_WINDOW_FEEL,
              B_NOT_ZOOMABLE | B_NOT_MINIMIZABLE | B_ASYNCHRONOUS_CONTROLS)
    , m_flossPalette(flossPalette)
    , m_flossIndex(0)
    , m_flossPaletteView(0)
    , m_document(document)
    , m_save(false)
{
    BAF::Button* ok_button;

    m_makerList = new BAF::PopUpMenuBar();
    for (unsigned int i = 0; i < makerNames.size(); ++i) {
        m_makerList->AddItem(new BMenuItem(makerNames[i], NULL));
    }

    BAF::Box* topview = new BAF::Box(
            NULL, B_PLAIN_BORDER, -1,
            new BAF::VGroup(
                    B_ALIGN_LEFT, new BAF::StringView("Current Floss Palette"),
                    new BAF::Box(NULL, B_PLAIN_BORDER, 0,
                            new BAF::ScrollView(m_flossPaletteView
                                         = new XSFlossPaletteView(&m_flossPalette, &m_flossIndex),
                                         B_WILL_DRAW, false, true)),
                    new BAF::HGroup(
                            B_ALIGN_TOP,
                            new BAF::Box("Available Colors", B_FANCY_BORDER, -1,
                                    new BAF::VGroup(B_ALIGN_LEFT,
                                                 new BAF::HGroup(B_ALIGN_MIDDLE, new BAF::HSpacer(),
                                                            new BAF::StringView("Floss List:"),
                                                            m_makerList, 0),
                                                 // new PaletteView(  #, description, color, ...
                                                 0)),
                            new BAF::Box("Palette Editor", B_FANCY_BORDER, -1,
                                    new BAF::HGroup(B_ALIGN_MIDDLE,
                                                 // new XSFlossView(),
                                                 // new BAF::SpinBox(1, 6),
                                                 new BAF::StringView("strands"),
                                                 // new BAF::PictButton(a "X" button, to remove this
                                                 // entry.
                                                 0)),
                            0),
                    new BAF::HGroup(B_ALIGN_MIDDLE, new BAF::HSpacer(),
                            ok_button = new BAF::Button("OK", BAF_MSG_BUTTON_OKAY),
                            new BAF::Button("Cancel", B_QUIT_REQUESTED), 0),
                    0));

    SetDefaultButton(ok_button);

    AddChild(dynamic_cast<BView*>(topview));

    BWindow* parentWindow = document->GetWindow();
    AddToSubset(parentWindow);

    ResizeToPreferred();
    BRect frame = BAF::Layout::PosChildOf(Frame(), parentWindow->Frame());
    MoveTo(frame.left, frame.top);

    Show();
}

XSEditFlossWindow::~XSEditFlossWindow()
{
}

bool XSEditFlossWindow::QuitRequested()
{
    if (m_save)
        ;  // FIXME:  save stuff from UI

    BMessage done(BAF_MSG_DIALOG_CLOSING);
    done.AddInt32("dialog", XS_DIALOG_EDIT_FLOSS);
    done.AddInt32("button", m_save ? BAF_MSG_BUTTON_OKAY : BAF_MSG_BUTTON_CANCEL);

    m_document->GetWindow()->PostMessage(&done);
    // Document is now free to grab data.
    // I await document's BAF_MSG_QUIT_REQUESTED signal to die...
    return false;
}

void XSEditFlossWindow::MessageReceived(BMessage* message)
{
    switch (message->what) {
    case BAF_MSG_QUIT_REQUESTED:
        Quit();
        break;
    case BAF_MSG_BUTTON_OKAY:
        m_save = true;
        PostMessage(B_QUIT_REQUESTED);
        break;
    }
}
