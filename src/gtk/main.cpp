#include "gtk/XSGtkApplication.h"
#include "gtk/XSGtkEditWindow.h"
#include "xs/XSController.h"

#include <gtkmm.h>

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
            "org.uscoffings.wfxs");

    XSGtkApplication xsapp(argc, argv);
    XSGtkEditWindow window;
    app->run(window);

    return 0;
}
