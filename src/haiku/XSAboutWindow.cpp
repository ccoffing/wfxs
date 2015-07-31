#include "XSAboutWindow.h"
#include "XSMessages.h"

#include <BAF/app/Messages.h>
#include <BAF/interface/LayoutAll.h>
#include <BAF/interface/Pos.h>
#include <app/Message.h>


XSAboutWindow::XSAboutWindow() :
    BAF::Window(BRect(), "About...", B_TITLED_WINDOW,
                BAF::WIN_AUTO_SIZE | BAF::WIN_ESCAPE_TO_CLOSE)
{
    BAF::Button *button;
    BAF::Box *topview = new BAF::Box(NULL, B_PLAIN_BORDER, -1,
            new BAF::VGroup(B_ALIGN_CENTER,
                    new BAF::StringView("Wild Flower Cross Stitch"),
                    new BAF::StringView("Version 1.0"),
                    new BAF::UrlView("http://uscoffings.net", "http://uscoffings.net"),
                    button = new BAF::Button("OK", BAF_MSG_BUTTON_OKAY),
                    0
                    )
            );

    SetDefaultButton(button);
    AddChild(dynamic_cast<BView *>(topview));
    ResizeToPreferred();
    BAF::Layout::PosCenterOnScreen(this);
    Show();
}

void XSAboutWindow::MessageReceived(BMessage *message)
{
    switch (message->what) {
    case BAF_MSG_BUTTON_OKAY:
        Quit();
        break;
    default:
        BWindow::MessageReceived(message);
    }
}
