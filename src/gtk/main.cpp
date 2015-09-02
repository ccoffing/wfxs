#include "xs/XSApplication.h"
#include "xs/XSController.h"
#include "gtk/XSGtkEditWindow.h"

#include <gtkmm.h>

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
            "org.uscoffings.wfxs");

    //new XSGtkApplication();

    XSGtkEditWindow window;
    app->run(window);

    return 0;
}
