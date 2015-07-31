#include "XSApplication.h"
#include "XSController.h"
#include "XSView_gtk.h"
#include "XSWindow_gtk.h"

#include <gtkmm.h>

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    new XSApplication();

    XSModel model(100, 100);
    XSView_gtk view;
    XSController controller(&model, &view);
    XSWindow_gtk window(controller);

    Gtk::Main::run(window.m_win);

    return 0;
}
