#ifndef _XS_FLOSS_PALETTE_VIEW_H
#define _XS_FLOSS_PALETTE_VIEW_H

/** @file
 *  @brief TODO
 */

#include <wx/scrolwin.h>

class XSFlossPalette;

/**
 *  @brief TODO
 */
class XSFlossPaletteView : public wxScrolledWindow {
public:
    XSFlossPaletteView(XSFlossPalette const*, unsigned int* currentIndex);

    void RefreshWell(unsigned int index);
    void RefreshWell(unsigned int wx, unsigned int wy);
    void RefreshAllWells();

    void OnDraw(wxDC& dc);
    void OnMouseEvent(wxMouseEvent& event);
    void ResetScrollBars();

private:
    void GetVerticalLayout(unsigned int* x, unsigned int* y);
    void GetHorizontalLayout(unsigned int* x, unsigned int* y);
    void DrawWell(wxDC& dc, float x, float y, wxColour const& color, bool hilite);
    void GetWellFromPoint(wxPoint p, unsigned int* x, unsigned int* y, bool*);
    void GetXY(unsigned int index, unsigned int* x, unsigned int* y);
    unsigned int GetIndex(unsigned int x, unsigned int y);

    XSFlossPalette const* m_flossPalette;
    float m_wellWidth;
    float m_wellHeight;
    float m_wellPadding;

    // The current layout
    unsigned int m_rows, m_cols;
    bool m_horizontal;
    unsigned int* m_currentIndex;

    // Disallow copying
    XSFlossPaletteView(XSFlossPaletteView const&);
    XSFlossPaletteView& operator=(XSFlossPaletteView&);
};

#endif
