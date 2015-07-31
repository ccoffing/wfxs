#include "XSApplication.h"
#include "XSMessages.h"
#include "XSStartupWizard.h"

#include <BAF/app/Messages.h>
#include <BAF/interface/LayoutAll.h>
#include <BAF/interface/Pos.h>
#include <app/Message.h>


XSStartupWizard::XSStartupWizard() :
    BAF::Window(BRect(), "Welcome...", B_TITLED_WINDOW,
                BAF::WIN_AUTO_SIZE | BAF::WIN_ESCAPE_TO_CLOSE)
{
    BAF::Button *button;
    BAF::Box *topview = new BAF::Box(NULL, B_PLAIN_BORDER, -1,
            new BAF::VGroup(B_ALIGN_CENTER,
                    button = new BAF::Button("New...", BAF_MSG_NEW),
                    new BAF::Button("Open...", BAF_MSG_OPEN),
                    new BAF::Button("Quit", BAF_MSG_QUIT_APP),
                    0
                    )
            );

    SetDefaultButton(button);
    AddChild(dynamic_cast<BView *>(topview));
    ResizeToPreferred();
    BAF::Layout::PosCenterOnScreen(this);
    Show();
}

void XSStartupWizard::MessageReceived(BMessage *message)
{
    switch (message->what) {
    case BAF_MSG_NEW:
        xs_app->PostMessage(BAF_MSG_NEW);
        Close();
        break;
    case BAF_MSG_OPEN:
        xs_app->PostMessage(BAF_MSG_OPEN);
        Close();
        break;
    case BAF_MSG_QUIT_APP:
        xs_app->PostMessage(B_QUIT_REQUESTED);
        break;
    default:
        BWindow::MessageReceived(message);
    }
}
