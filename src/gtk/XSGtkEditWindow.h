#ifndef XS_GTK_EDIT_WINDOW_H
#define XS_GTK_EDIT_WINDOW_H

#include "xs/XSController.h"
#include "gtk/XSGtkCanvas.h"

#include <gtkmm/hvbox.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>

class XSController;

class XSGtkEditWindow : public Gtk::Window {
public:
    XSGtkEditWindow();
    virtual ~XSGtkEditWindow();

protected:
    void onNew();
    void onOpen();

    XSModel m_model;
    XSController m_controller;

    Gtk::VBox m_box;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
};

#endif
