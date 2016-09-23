#ifndef _XS_CANVAS_H
#define _XS_CANVAS_H

/** @file
 *  @brief The main cross stitch canvas
 */

#include <wx/gdicmn.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

#include "XSStitchTypes.h"
#include "XSToolTypes.h"

class XSSquareIO;
class XSDocChild;
class XSFlossPalette;
class XSSelectionPath;
namespace mt {
class BitMask;
};


/**
 *  @brief The main cross stitch canvas
 */
class XSCanvas : public wxScrolledWindow {
public:
    XSCanvas(wxWindow* parent, XSDocChild* docchild);

    void RefreshSquare(unsigned int x, unsigned int y);
    void RefreshSquareContents(unsigned int x, unsigned int y);
    void RefreshSquares(
            unsigned int startX, unsigned int startY, unsigned int stopX, unsigned int stopY);
    void KeyboardClick();
    static unsigned int GetPreferredZoom();
    void AdjustScrollBars();
    void ScrollBy(int dx, int dy);
    void MoveCursorBy(int dx, int dy);
    bool Layout();

protected:
    virtual ~XSCanvas();

    void OnDraw(wxDC& dc);
    void OnKeyDown(wxKeyEvent& event);
    void OnMouseEvent(wxMouseEvent& event);
    void MouseDown(unsigned int x, unsigned int y, unsigned int region = 6, bool cmdDown = false);
    void MouseUp(unsigned int x, unsigned int y, unsigned int region = 6);
    void MouseMoved(unsigned int x, unsigned int y, unsigned int region = 6, bool cmdDown = false);

    void DrawDesignSquare(wxDC& dc, XSSquareIO& square, float zx, float zy, unsigned int zoom,
            XSFlossPalette const& flossPalette);
    void DrawPatternSquare(wxDC& dc, XSSquareIO& square, float zx, float zy, unsigned int zoom,
            XSFlossPalette const& flossPalette);
    void DrawRealisticSquare(wxDC& dc, XSSquareIO& square, float zx, float zy, unsigned int zoom,
            XSFlossPalette const& flossPalette);
    void ClearToSquare(unsigned int nextX, unsigned int nextY);
    const wxRect SquareInnerRect(unsigned int x, unsigned int y);

private:
    XSDocChild* m_docchild;

    unsigned int cursorX, cursorY;
    bool m_resetCursor;

    bool m_mouseDown;
    bool m_mouseControl;
    unsigned int m_prevX, m_prevY, m_prevRegion;

    XSSelectionPath* m_selectionPath;
    mt::BitMask* m_selectionMask;

    // Disallow copying
    XSCanvas(XSCanvas const&);
    XSCanvas& operator=(XSCanvas&);

    DECLARE_EVENT_TABLE()
};

class XSCanvasPage : public wxWindow {
public:
    XSCanvasPage(wxWindow* parent, XSDocChild* docchild)
        : wxWindow(parent, wxID_ANY)
    {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* topsizer = new wxStaticBoxSizer(wxVERTICAL, panel, wxT("testing"));

        // FIXME
        topsizer->Add(new wxTextCtrl(parent, -1, wxT("My text."), wxDefaultPosition,
                              wxSize(100, 60), wxTE_MULTILINE),
                2, wxEXPAND);
        topsizer->Add(new wxTextCtrl(parent, -1, wxT("My text."), wxDefaultPosition,
                              wxSize(100, 60), wxTE_MULTILINE),
                1, wxEXPAND);
        topsizer->Add((m_canvas = new XSCanvas(parent, docchild)), 0, wxEXPAND | wxALL, 10);
        SetSizer(topsizer);
        topsizer->SetSizeHints(parent);
    }
    inline XSCanvas* GetCanvas()
    {
        return m_canvas;
    }

private:
    XSCanvas* m_canvas;
};

#endif
