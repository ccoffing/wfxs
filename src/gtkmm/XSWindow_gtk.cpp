#include "XSController.h"
#include "XSWindow_gtk.h"
#include "clc/data/Buffer.h"

#include <iostream>


XSGtkWindow::XSGtkWindow(XSController &controller) :
    m_controller(controller)
{
    add(m_box);

    m_refActionGroup = Gtk::ActionGroup::create();
    m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
            sigc::mem_fun(*this, &XSGtkWindow::onNew));
    m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
            sigc::mem_fun(*this, &XSGtkWindow::onOpen));

#if 1
    XSWindow::GetPreferredFrame(x, y);
    set_default_size(XSWindow::);

    // Sets the border width of the window.
    set_border_width(10);

    // When the button receives the "clicked" signal, it will call the
    // on_button_clicked() method defined below.
    m_button.signal_clicked().connect(sigc::mem_fun(*this,
                    &XSWindow::on_button_clicked));

    // This packs the button into the Window (a container).
    add(m_button);

    // The final step is to display this newly created widget...
    m_button.show();
#endif
    m_box.show();
}

XSGtkWindow::~XSGtkWindow()
{
}

void XSGtkWindow::onNew()
{
    m_controller.OnNew();
}

void XSGtkWindow::onOpen()
{
    // dialog...
    //
    // m_controller.onOpen(filename);
}

XSWindow_gtk::XSWindow_gtk(XSController &controller) :
    m_win(controller)
{
}

void XSWindow_gtk::setTitle(clc::Buffer const &title)
{
    m_win.set_title(title.c_str());
}
