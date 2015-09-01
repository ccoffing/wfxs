#ifndef GTKMM_XSWINDOW_H
#define GTKMM_XSWINDOW_H

#include "XSWindow.h"

#include <gtkmm.h>

class XSController;

class XSGtkWindow : public Gtk::Window {
public:
    XSGtkWindow(XSController &controller);
    virtual ~XSGtkWindow();

protected:
    void onNew();
    void onOpen();

    Gtk::VBox m_box;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    XSController &m_controller;
};


/**
 *  proxy class
 */
class XSWindow_gtk : public XSWindow {
public:
    XSWindow_gtk(XSController &controller);

    void setTitle(const std::string &title);
    void setSize()
    {
    }

    void setPosition()
    {
    }

    XSGtkWindow m_win;
};

#endif
