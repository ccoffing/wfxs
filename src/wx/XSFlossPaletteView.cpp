#include <wx/colour.h>
#include <wx/dc.h>

#include "XSFloss.h"
#include "XSFlossPalette.h"
#include "XSFlossPaletteView.h"


static wxColour black(0, 0, 0);
static wxColour white(0xff, 0xff, 0xff);


XSFlossPaletteView::XSFlossPaletteView(
        XSFlossPalette const* flossPalette, unsigned int* currentIndex)
    : wxScrolledWindow()
    , m_flossPalette(flossPalette)
    , m_wellWidth(20)
    , m_wellHeight(20)
    , m_wellPadding(5)
    , m_rows(0)
    , m_cols(0)
    , m_horizontal(true)
    , m_currentIndex(currentIndex)
{
    // FIXME:  do this post-init
    // ResetScrollBars();
}


void XSFlossPaletteView::RefreshWell(unsigned int index)
{
    unsigned int x, y;

    GetXY(index, &x, &y);
    RefreshWell(x, y);
}


void XSFlossPaletteView::RefreshWell(unsigned int x, unsigned int y)
{
    int lx = m_wellPadding + x * (m_wellWidth + m_wellPadding);
    int ly = m_wellPadding + y * (m_wellHeight + m_wellPadding);
    int px, py;

    CalcScrolledPosition(lx, ly, &px, &py);
    wxRect r(px, py, px + m_wellWidth, py + m_wellHeight);
    Refresh(true, &r);
}


void XSFlossPaletteView::RefreshAllWells()
{
    ResetScrollBars();
    Refresh();  // Number of wells may have changed, even if layout didn't
}


void XSFlossPaletteView::OnMouseEvent(wxMouseEvent& event)
{
    int lx, ly;

    CalcUnscrolledPosition(event.GetX(), event.GetY(), &lx, &ly);

    unsigned int x, y;
    bool valid;
    GetWellFromPoint(wxPoint(lx, ly), &x, &y, &valid);
    if (!valid)
        return;

    // Refresh old well
    unsigned int wx, wy;
    GetXY(*m_currentIndex, &wx, &wy);
    RefreshWell(wx, wy);

    // Refresh new well
    *m_currentIndex = GetIndex(x, y);
    RefreshWell(x, y);
}


void XSFlossPaletteView::DrawWell(wxDC& dc, float x, float y, wxColour const& color, bool highlight)
{
    dc.SetBrush(wxBrush(color));
    dc.SetPen(wxPen(black, highlight ? 3.0 : 1.0));
    dc.DrawRoundedRectangle(x, y, m_wellWidth, m_wellHeight, m_wellPadding);
}


void XSFlossPaletteView::GetXY(unsigned int index, unsigned int* x, unsigned int* y)
{
    if (m_horizontal) {
        *x = (index % m_cols);
        *y = index / m_cols;
    } else {
        *x = index / m_rows;
        *y = (index % m_rows);
    }
}


unsigned int XSFlossPaletteView::GetIndex(unsigned int x, unsigned int y)
{
    if (m_horizontal)
        return y * m_cols + x;
    else
        return x * m_rows + y;
}


void XSFlossPaletteView::GetWellFromPoint(
        wxPoint p, unsigned int* wx, unsigned int* wy, bool* valid)
{
    unsigned int x = ((unsigned int)(p.x + m_wellPadding / 2.0))
            / (unsigned int)(m_wellWidth + m_wellPadding);
    unsigned int y = ((unsigned int)(p.y + m_wellPadding / 2.0))
            / (unsigned int)(m_wellHeight + m_wellPadding);

    *valid = (GetIndex(x, y) < m_flossPalette->size());

    *wx = x;
    *wy = y;
}


void XSFlossPaletteView::OnDraw(wxDC& dc)
{
    //    printf("XSFlossPaletteView::Draw...\n");
    wxCoord x, y, width, height;

    dc.GetClippingBox(&x, &y, &width, &height);
    wxRect updateRect(x, y, width, height);

    dc.SetPen(wxPen(white));
    dc.SetBrush(wxBrush(white));
    dc.DrawRectangle(updateRect);

    unsigned int wells = m_flossPalette->size();
    unsigned int startX, startY;
    unsigned int stopX, stopY;
    bool valid;
    int lx, ly;
    CalcUnscrolledPosition(x, y, &lx, &ly);
    GetWellFromPoint(wxPoint(lx, ly), &startX, &startY, &valid);
    CalcUnscrolledPosition(x + width, y + height, &lx, &ly);
    GetWellFromPoint(wxPoint(lx, ly), &stopX, &stopY, &valid);
    if (stopY >= m_rows)
        stopY = m_rows ? m_rows - 1 : 0;
    if (stopX >= m_cols)
        stopX = m_cols ? m_cols - 1 : 0;
    // printf("starty %d  stopy %d\n", startY, stopY);
    // printf("startx %d  stopx %d\n", startX, stopX);
    for (unsigned int y = startY; y <= stopY; y++) {
        for (unsigned int x = startX; x <= stopX; x++) {
            unsigned int i = y * m_cols + x;
            if (i >= wells)
                continue;  // partial final column or row
            DrawWell(dc, m_wellPadding + x * (m_wellWidth + m_wellPadding),
                    m_wellPadding + y * (m_wellHeight + m_wellPadding),
                    m_flossPalette->GetFloss(i).GetColor(), (i == *m_currentIndex));
        }
    }
    //    printf("XSFlossPaletteView::Draw done!\n");
}


void XSFlossPaletteView::GetHorizontalLayout(unsigned int* x, unsigned int* y)
{
    int w, h;

    GetVirtualSize(&w, &h);
    unsigned int wells = m_flossPalette->size();
    *y = (unsigned int)((h - m_wellPadding) / (m_wellHeight + m_wellPadding));
    if (*y == 0)
        *y = 1;
    *x = (wells + (*y) - 1) / (*y);
}


void XSFlossPaletteView::GetVerticalLayout(unsigned int* x, unsigned int* y)
{
    int w, h;

    GetVirtualSize(&w, &h);
    unsigned int wells = m_flossPalette->size();
    *x = (unsigned int)((w - m_wellPadding) / (m_wellWidth + m_wellPadding));
    if (*x == 0)
        *x = 1;
    *y = (wells + (*x) - 1) / (*x);
}


void XSFlossPaletteView::ResetScrollBars()
{
    unsigned int x, y;

    if (m_horizontal)
        GetHorizontalLayout(&x, &y);
    else
        GetVerticalLayout(&x, &y);

    if (x != m_cols || y != m_rows) {
        m_cols = x;
        m_rows = y;
    }

    SetScrollbars(1, 1, m_wellPadding + (x * (m_wellPadding + m_wellWidth)),
            m_wellPadding + (y * (m_wellPadding + m_wellHeight)));
}
