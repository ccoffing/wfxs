#ifndef GTKMM_XSVIEW_GTK_H
#define GTKMM_XSVIEW_GTK_H

#include "XSView.h"

class XSView_gtk : public XSView {
public:
    XSView_gtk();
    ~XSView_gtk();

    void AdjustScrollBars();

    void MoveCursorBy(int dx, int dy);

    void Refresh();

    void RefreshSquareContents(unsigned int x, unsigned int y);

    void ScrollBy(int dx, int dy);
};

#endif
