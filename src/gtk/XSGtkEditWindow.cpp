#include "xs/XSController.h"
#include "gtk/XSGtkEditWindow.h"

#include <iostream>


XSGtkEditWindow::XSGtkEditWindow() :
    m_model(40, 40),
    m_controller(&m_model)
{
    add(m_box);

    m_refActionGroup = Gtk::ActionGroup::create();
    m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
            sigc::mem_fun(*this, &XSGtkEditWindow::onNew));
    m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
            sigc::mem_fun(*this, &XSGtkEditWindow::onOpen));

#if 0
    m_controller.GetPreferredFrame(x, y);
    set_default_size(x, y);

    // Sets the border width of the window.
    set_border_width(10);

    // When the button receives the "clicked" signal, it will call the
    // on_button_clicked() method defined below.
    m_button.signal_clicked().connect(sigc::mem_fun(*this, &on_button_clicked));

    // This packs the button into the Window (a container).
    add(m_button);

    // The final step is to display this newly created widget...
    m_button.show();
#endif
    m_box.show();
}

XSGtkEditWindow::~XSGtkEditWindow()
{
}

void XSGtkEditWindow::onNew()
{
    // TODO
}

void XSGtkEditWindow::onOpen()
{
    // TODO
}
