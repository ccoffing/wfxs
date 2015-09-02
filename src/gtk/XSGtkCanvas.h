#ifndef XS_GTK_CANVAS_H
#define XS_GTK_CANVAS_H

#include <gtkmm/drawingarea.h>

class XSGtkCanvas : public Gtk::DrawingArea {
public:
    XSGtkCanvas();
    ~XSGtkCanvas();
};

#endif
