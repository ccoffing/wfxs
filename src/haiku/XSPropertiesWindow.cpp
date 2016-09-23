#include <BAF/app/Messages.h>
#include <BAF/interface/LayoutAll.h>
#include <BAF/interface/Pos.h>
#include <app/AppDefs.h>
#include <app/Message.h>
#include <support/Debug.h>

#include "XSApplication.h"
#include "XSDocument.h"
#include "XSMessages.h"
#include "XSPropertiesWindow.h"


XSPropertiesWindow::XSPropertiesWindow(XSDocument* document, XSProperties const& properties)
    : BAF::Window(BRect(), "Properties", B_TITLED_WINDOW_LOOK, B_MODAL_SUBSET_WINDOW_FEEL,
              B_NOT_ZOOMABLE | B_NOT_MINIMIZABLE | B_ASYNCHRONOUS_CONTROLS)
    , m_properties(properties)
    , m_document(document)
    , m_save(false)
{
    BAF::Button* ok_button;
    BAF::TabView* tab_view;

    BAF::Box* topview = new BAF::Box(
            NULL, B_PLAIN_BORDER, -1,
            new BAF::VGroup(B_ALIGN_LEFT,
                    tab_view = new BAF::TabView(
                            "Pattern Size", new BAF::VGroup(B_ALIGN_LEFT, 0), "Description",
                            new BAF::VGroup(B_ALIGN_LEFT,
                                    m_title = new BAF::TextControl("Title:", ""),
                                    m_author = new BAF::TextControl("Author:", ""),
                                    m_fabric = new BAF::TextControl("Fabric:", ""), 0),
                            "Instructions",
                            new BAF::VGroup(B_ALIGN_LEFT, m_instructions = new BAF::TextView(), 0),
                            0),
                    new BAF::HGroup(B_ALIGN_MIDDLE, new BAF::HSpacer(),
                                    ok_button = new BAF::Button("OK", BAF_MSG_BUTTON_OKAY),
                                    new BAF::Button("Cancel", B_QUIT_REQUESTED), 0),
                    0));

    NormalizeTextControls(m_title, m_author, m_fabric, 0);

    SetDefaultButton(ok_button);

    SetProperties();

    AddChild(dynamic_cast<BView*>(topview));

    BWindow* parentWindow = document->GetWindow();
    AddToSubset(parentWindow);

    ResizeToPreferred();
    BRect frame = BAF::Layout::PosChildOf(Frame(), parentWindow->Frame());
    MoveTo(frame.left, frame.top);

    Show();


#if 0
    BButton *button;
    BRect bounds = Bounds();
    BBox *parent = new BBox(bounds, "ParentView", B_FOLLOW_ALL, B_PLAIN_BORDER);
    AddChild(parent);

    bounds.top += 10;
    bounds.bottom -= 45;
    BTabView *tabview = new BTabView(bounds, "TabView");
    tabview->SetFont(be_plain_font);
    tabview->SetViewColor(parent->ViewColor());
// what's the point of this?
//    tabview->ContainerView()->SetViewColor(parent->ViewColor());
    parent->AddChild(tabview);

    BRect viewsize = tabview->ContainerView()->Bounds();
    viewsize.InsetBy(5, 5);

    BView *view = new BView(viewsize, "Pattern Size", B_FOLLOW_ALL, 0);
    view->SetViewColor(tabview->ViewColor());
    tabview->AddTab(view);

#if 0
    view = new BView(viewsize, "Description", B_FOLLOW_ALL, 0);
    view->SetViewColor(tabview->ViewColor());
    BRect wr = viewsize;
    wr.InsetBy(10, 10);
    BPoint cursor(wr.left, wr.top);
    float labelWidth = 0.0;
    float totalWidth = wr.Width();
    m_title = new BAF::TextControl("Title:", "", labelWidth, totalWidth, cursor);
    cursor = BAF::Layout::PosBelow(*m_title);
    m_author = new BAF::TextControl("Author:", "", labelWidth, totalWidth, cursor);
    cursor = BAF::Layout::PosBelow(*m_author);
    m_fabric = new BAF::TextControl("Fabric:", "", labelWidth, totalWidth, cursor);

    m_title->SetDivider(labelWidth);
    view->AddChild(m_title);
    m_author->SetDivider(labelWidth);
    view->AddChild(m_author);
    m_fabric->SetDivider(labelWidth);
    view->AddChild(m_fabric);
#else
    float labelWidth = 0.0;
    float totalWidth = viewsize.Width();
    view = new BAF::VGroup(B_ALIGN_CENTER,
            m_title = new BAF::TextControl("Title:", "", labelWidth, totalWidth),
            m_author = new BAF::TextControl("Author:", "", labelWidth, totalWidth),
            m_fabric = new BAF::TextControl("Fabric:", "", labelWidth, totalWidth),
            0
            );

    m_title->SetDivider(labelWidth);
    m_author->SetDivider(labelWidth);
    m_fabric->SetDivider(labelWidth);
#endif
    tabview->AddTab(view);

#if 0
    view = new BView(viewsize, "Instructions", B_FOLLOW_ALL, 0);
    view->SetViewColor(tabview->ViewColor());
    wr = viewsize;
    BRect tvr = wr;
    tvr.OffsetTo(0, 0);
    view->AddChild(new BTextView(wr, "Instructions", tvr, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE));
#else
#endif
    tabview->AddTab(view);

#if 0
    bounds = Bounds();
    button = new BButton(BRect(bounds.right - 100, bounds.bottom - 30, bounds.right - 40, bounds.bottom - 10),
            "Cancel", "Cancel", new BMessage(B_QUIT_REQUESTED), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
    parent->AddChild(button);
    button = new BButton(BRect(bounds.right - 200, bounds.bottom - 30, bounds.right - 140, bounds.bottom - 10),
            "Okay", "Okay", new BMessage(BAF_MSG_BUTTON_OKAY), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
    parent->AddChild(button);
    SetDefaultButton(button);
#else
    new BAF::HGroup(B_ALIGN_TOP,
            new BAF::Button("Cancel", B_QUIT_REQUESTED),
            button = new BAF::Button("Okay", BAF_MSG_BUTTON_OKAY));
    SetDefaultButton(button);
#endif

    SetProperties();

    BWindow *parentWindow = document->GetWindow();
    AddToSubset(parentWindow);
    BRect frame = BAF::Layout::PosChildOf(Frame(), parentWindow->Frame());
    MoveTo(frame.left, frame.top);

    Show();
#endif
}

void XSPropertiesWindow::GetProperties()
{
    m_properties.m_title = m_title->Text();
    m_properties.m_author = m_author->Text();
    m_properties.m_fabric = m_fabric->Text();
    m_properties.m_instructions = m_instructions->Text();
}

void XSPropertiesWindow::SetProperties()
{
    m_title->SetText(m_properties.m_title.String());
    m_author->SetText(m_properties.m_author.String());
    m_fabric->SetText(m_properties.m_fabric.String());
}

bool XSPropertiesWindow::QuitRequested()
{
    if (m_save)
        GetProperties();

    BMessage done(BAF_MSG_DIALOG_CLOSING);
    done.AddInt32("dialog", XS_DIALOG_PROPERTIES);
    done.AddInt32("button", m_save ? BAF_MSG_BUTTON_OKAY : BAF_MSG_BUTTON_CANCEL);

    m_document->GetWindow()->PostMessage(&done);
    // Document is now free to grab m_properties.
    // I await document's BAF_MSG_QUIT_REQUESTED signal to die...
    return false;
}

void XSPropertiesWindow::MessageReceived(BMessage* message)
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
