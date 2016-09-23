#include <vector>

#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/pen.h>

#include <mt/algorithm/BresenhamLine.h>

#include "XSCanvas.h"
#include "XSDocChild.h"
#include "XSDocument.h"
#include "XSFloss.h"
#include "XSFlossPalette.h"
#include "XSSelection.h"
#include "XSSelectionPath.h"
#include "XSSquareIO.h"
#include "XSToolTypes.h"


BEGIN_EVENT_TABLE(XSCanvas, wxScrolledWindow)
EVT_KEY_DOWN(XSCanvas::OnKeyDown)
EVT_MOUSE_EVENTS(XSCanvas::OnMouseEvent)
END_EVENT_TABLE()


static wxColour black(0, 0, 0, 0xff);
static wxColour cloth_color(0xef, 0xef, 0xd0, 0xff);
static wxColour cursor_color(0, 0, 0xff, 0xff);
static wxColour dark_gray(0x30, 0x30, 0x30, 0xff);
static wxColour gray(0xa0, 0xa0, 0xa0, 0xff);


/*
 *  Here is the subdivision for embelishments (knots, beads, etc).  Note that
 *  "R" means the spot is handled by the square to the right; "B" means the
 *  square below, and "D" means the diagonal square.
 *    0---1---R
 *    | 3   4 |
 *    5   6   R
 *    | 8   9 |
 *    B---B---D
 *
 *  Backstitches, also, can start or end at any of these locations.
 */
struct RegionOffsetDef {
    unsigned int xOffset;
    unsigned int yOffset;
    unsigned int newX;
    unsigned int newY;
    unsigned int newRegion;
};

static struct RegionOffsetDef RegionOffsets[] =
{
    { 0, 0, 0, 0, 0 },
    { 2, 0, 0, 0, 1 },
    { 0, 0, 1, 0, 0 },
    { 1, 1, 0, 0, 3 },
    { 3, 1, 0, 0, 4 },
    { 0, 2, 0, 0, 5 },
    { 2, 2, 0, 0, 6 },
    { 0, 2, 1, 0, 5 },
    { 1, 3, 0, 0, 8 },
    { 3, 3, 0, 0, 9 },
    { 0, 0, 0, 1, 0 },
    { 2, 0, 0, 1, 1 },
    { 0, 0, 1, 1, 0 }
};


/*
 *  To understand this function, don't think of a square like this:
 *    0---1---2
 *    | 3   4 |
 *    5   6   7
 *    | 8   9 |
 *   10--11--12
 *
 *  But rather one like this:
 *   / \ / \ / \
 *  X 0-X-1-X-2 X
 *   \|/ \ / \|/
 *    X 3 X 4 X
 *   /|\ / \ /|\
 *  X 5 X 6 X 7 X
 *   \|/ \ / \|/
 *    X 8 X 9 X
 *   /|\ / \ /|\
 *  X10-X11-X12 X
 *   \ / \ / \ /
 *
 *  The inequalities below describe the diagonal lines.  If the user
 *  clicks anywhere in the diamond, it is attributed to that region.
 */
static unsigned int XYPercentToRegion(unsigned int xPercent, unsigned int yPercent)
{
    int r;
    int intercept1 = xPercent + yPercent;
    int intercept2 = xPercent - yPercent;

    // First lower-left-to-upper-right diagonal
    if (intercept1 < 25)
        r = 0;
    // Second diagonal
    else if (intercept1 < 75) {
        if (intercept2 > 25)
            r = 1;
        else if (intercept2 > -25)
            r = 3;
        else
            r = 5;
    }
    // Third diagonal
    else if (intercept1 < 125) {
        if (intercept2 > 75)
            r = 2;
        else if (intercept2 > 25)
            r = 4;
        else if (intercept2 > -25)
            r = 6;
        else if (intercept2 > -75)
            r = 8;
        else
            r = 10;
    }
    // Forth diagonal
    else if (intercept1 < 175) {
        if (intercept2 > 25)
            r = 7;
        else if (intercept2 > -25)
            r = 9;
        else
            r = 11;
    }
    // Fifth diagonal
    else
        r = 12;
    return r;
}


XSCanvas::XSCanvas(wxWindow* parent, XSDocChild* docchild)
    : wxScrolledWindow(parent, wxID_ANY)
    , m_docchild(docchild)
    , cursorX(0)
    , cursorY(0)
    , m_resetCursor(true)
    , m_mouseDown(false)
    , m_mouseControl(true)
    , m_prevX((unsigned int)-1)
    , m_prevY((unsigned int)-1)
    , m_prevRegion((unsigned int)-1)
    , m_selectionPath(0)
    , m_selectionMask(0)
{
    AdjustScrollBars();
    SetFocus();
}


XSCanvas::~XSCanvas()
{
}


unsigned int XSCanvas::GetPreferredZoom()
{
    return 8;
}


void XSCanvas::KeyboardClick()
{
    unsigned int zoom = m_docchild->GetDocument()->GetZoom();
    wxPoint p(cursorX * zoom, cursorY * zoom);

    MouseDown(cursorX, cursorY);
    MouseUp(cursorX, cursorY);
    m_mouseControl = false;
}


bool XSCanvas::Layout()
{
    AdjustScrollBars();
    return true;
}


void XSCanvas::RefreshSquares(
        unsigned int startX, unsigned int startY, unsigned int stopX, unsigned int stopY)
{
    unsigned int zoom = m_docchild->GetDocument()->GetZoom();
    wxRect r(startX * zoom, startY * zoom, stopX * zoom + zoom, stopY * zoom + zoom);

    Refresh(true, &r);
}


void XSCanvas::RefreshSquareContents(unsigned int x, unsigned int y)
{
    wxASSERT(x < m_docchild->GetDocument()->SquaresX());
    wxASSERT(y < m_docchild->GetDocument()->SquaresY());
    wxRect r = SquareInnerRect(x, y);
    Refresh(true, &r);
}


void XSCanvas::RefreshSquare(unsigned int x, unsigned int y)
{
    XSDocument const* doc = m_docchild->GetDocument();

    wxASSERT(x < doc->SquaresX());
    wxASSERT(y < doc->SquaresY());

    unsigned int zoom = doc->GetZoom();
    wxRect r(x * zoom, y * zoom, x * zoom + zoom, y * zoom + zoom);
    Refresh(true, &r);
}


const wxRect XSCanvas::SquareInnerRect(unsigned int x, unsigned int y)
{
    XSDocument* doc = m_docchild->GetDocument();
    unsigned int zoom = doc->GetZoom();

    return wxRect(x * zoom + 1, y * zoom + 1, x * zoom + zoom - 1, y * zoom + zoom - 1);
}


#if 0
XSSquareIO XSCanvas::CalculateSquare(unsigned int x,
        unsigned int y,
        unsigned int xPercent,
        unsigned int yPercent,
        ToolType toolType)
{
    switch (toolType) {
    case ToolType_Stitch:
    {
        xxx
        break;
    }
    case ToolType_Knot:
    {
        unsigned int region = XYPercentToRegion(xPercent, yPercent);
        struct RegionOffsetDef *ro = &RegionOffsets[region];
        xxx m_docchild->SetKnot(x + ro->newX, y + ro->newY, ro->newRegion);
        break;
    }
    case ToolType_Bead:
    {
        unsigned int region = XYPercentToRegion(xPercent, yPercent);
        struct RegionOffsetDef *ro = &RegionOffsets[region];
        xxx m_docchild->SetBead(x + ro->newX, y + ro->newY, ro->newRegion);
        break;
    }
    default:
    {
        wxASSERT(0);
        break;
    }
    }
    return xxx;
}
#endif


static bool DeJiggle(unsigned int x, unsigned int y, bool reset)
{
    /*
     *  I'm playing a little fast-and-loose by having these global-static
     *  (shared between instances) but you'll never be able to drag from
     *  one window to the same square on another (there will be a MouseDown
     *  event, which will reset these).
     */
    static unsigned int previousX, previousY = (unsigned int)-1;

    if (!reset) {
        if (previousX == x && previousY == y)
            return false;
    }
    previousX = x;
    previousY = y;
    return true;
}


void XSCanvas::ClearToSquare(unsigned int nextX, unsigned int nextY)
{
    wxASSERT(!(m_prevX == nextX && m_prevY == nextY));  // de-jiggled

    std::vector<wxPoint> points;
    mt::BresenhamLine<wxPoint>(m_prevX, m_prevY, nextX, nextY, points);

    // First point has already been cleared by MouseDown
    wxASSERT(points.size() > 1);
    points.erase(points.begin());

    m_docchild->OnClearSquares(points);
}


void XSCanvas::OnMouseEvent(wxMouseEvent& event)
{
    if (!m_docchild->IsActive())
        return;

    wxClientDC dc(this);
    DoPrepareDC(dc);
    wxPoint point = event.GetLogicalPosition(dc);
    unsigned int zoom = m_docchild->GetDocument()->GetZoom();
    unsigned int x = ((unsigned int)(point.x / zoom));
    unsigned int y = ((unsigned int)(point.y / zoom));
    unsigned int xPercent = ((unsigned int)point.x) % zoom * 100 / zoom;
    unsigned int yPercent = ((unsigned int)point.y) % zoom * 100 / zoom;
    unsigned int region = XYPercentToRegion(xPercent, yPercent);
    bool valid = (x < m_docchild->GetDocument()->SquaresX()
            && y < m_docchild->GetDocument()->SquaresY());
    bool newSquare = (cursorX != x || cursorY != y);

    ToolType toolType = m_docchild->GetToolType();
    if (valid && newSquare && ToolTypeUsesCursor(toolType)) {
        RefreshSquare(cursorX, cursorY);  // Erase old cursor
        cursorX = x;
        cursorY = y;
        //  Only give control back to the mouse if it moves from one
        //  square to another, NOT merely if it jiggles in a square
        //  different than the keyboard's square.
        m_mouseControl = true;
        RefreshSquare(cursorX, cursorY);  // to draw the cursor
    }

    if (!m_mouseDown && event.LeftIsDown())
        MouseDown(x, y, region, event.CmdDown());
    else if (m_mouseDown && !event.LeftIsDown())
        MouseUp(x, y, region);
    else {
        if (m_prevX != x || m_prevY != y || m_prevRegion != region)
            MouseMoved(x, y, region, event.CmdDown());
    }

    if (valid) {
        m_prevX = x;
        m_prevY = y;
        m_prevRegion = region;
    }
}


void XSCanvas::MouseMoved(unsigned int x, unsigned int y, unsigned int region, bool cmdDown)
{
    XSToolState const toolState = m_docchild->GetDocument()->ToolState();

    switch (toolState.m_toolType) {
    case ToolType_Stitch: {
        DeJiggle(x, y, true);
        //            wxASSERT(! (m_prevX == nextX && m_prevY == nextY));  // de-jiggled
        if (!m_mouseDown)
            break;
        std::vector<wxPoint> points;
        mt::BresenhamLine<wxPoint>(m_prevX, m_prevY, x, y, points);

        // First point has already been stitched by MouseDown
        wxASSERT(points.size() > 1);
        points.erase(points.begin());

        if (cmdDown)
            m_docchild->OnClearSquares(points);
        else
            m_docchild->OnSetStitches(points);
        break;
    }
    default:
        // FIXME:  Handle other types, as appropriate
        break;
    }
}


void XSCanvas::MouseDown(unsigned int x, unsigned int y, unsigned int region, bool cmdDown)
{
    XSToolState const toolState = m_docchild->GetDocument()->ToolState();

    m_mouseDown = true;

    switch (toolState.m_toolType) {
    case ToolType_Stitch:
        DeJiggle(x, y, true);
        if (cmdDown)
            m_docchild->OnClearSquare(x, y);
        else
            // FIXME:  pass region instead
            m_docchild->OnSetStitch(x, y, 50, 50);  // xPercent, yPercent);
        break;
    case ToolType_Bead: {
        DeJiggle(x, y, true);
        struct RegionOffsetDef* ro = &RegionOffsets[region];
        m_docchild->OnSetBead(x + ro->newX, y + ro->newY, ro->newRegion);
        break;
    }
    case ToolType_Knot: {
        DeJiggle(x, y, true);
        struct RegionOffsetDef* ro = &RegionOffsets[region];
        m_docchild->OnSetKnot(x + ro->newX, y + ro->newY, ro->newRegion);
        break;
    }
    case ToolType_Eraser:
        DeJiggle(x, y, true);
        m_docchild->OnClearSquare(x, y);
        break;
    case ToolType_FloodFill: {
        XSSquareIO oldSquare;
        m_docchild->GetDocument()->GetSquareData(
                &oldSquare, x, y, m_docchild->GetDocument()->GetCurrentLayerIndex());
#if 0
// zzz   seems like I need a CalculateSquare, similar to setSquare, but
//      it directly calls the doc functions, saves the resulting square,
//      and then undoes it.  Yuck
        XSSquareIO newSquare = doc->CalculateSquare(x, y,
                ((unsigned int)point.x) % zoom * 100 / zoom,
                ((unsigned int)point.y) % zoom * 100 / zoom,
                toolState.m_toolType, true, false);

        m_docchild->CommandFloodFill(x, y, oldSquare, newSquare);
#endif
        break;
    }
    case ToolType_Select:
        if (toolState.m_tool == Tool_RectangularSelect)
            m_selectionPath = new XSRectangularSelectionPath();
        else {
            if (!m_selectionPath)
                m_selectionPath = new XSFreeFormSelectionPath();
            else {
                if (m_selectionPath->Complete()) {
                    m_selectionMask = m_selectionPath->GetMask();
                    // FIXME
                } else {
                    // FIXME:  warn user in status bar
                }
            }
        }
        break;
    case ToolType_ColorPicker: {
        XSSquareIO square;
        m_docchild->GetDocument()->GetSquareData(
                &square, x, y, m_docchild->GetDocument()->GetCurrentLayerIndex());
        //            if (square.num_beads
        // FIXME ... look at region; pick relevant stich
        break;
    }
    case ToolType_Backstitch:
        // FIXME
        break;
    case ToolType_Couching:
        // FIXME
        break;
    case ToolType_Text:
        // FIXME
        break;
    default:
        wxASSERT(0);
    }
}


void XSCanvas::MouseUp(unsigned int x, unsigned int y, unsigned int region)
{
    m_mouseDown = false;

    XSToolState const toolState = m_docchild->GetDocument()->ToolState();
    ToolType toolType = m_docchild->GetToolType();
    // UpdateToolState(toolType);

    switch (toolType) {
    case ToolType_Stitch:
    case ToolType_Knot:
    case ToolType_Bead:
    case ToolType_Eraser:
        // Do nothing...
        break;
    case ToolType_Backstitch:
        // FIXME
        break;
    case ToolType_Couching:
        // FIXME
        break;
    case ToolType_Select:
        wxASSERT(m_selectionPath);
        if (toolState.m_tool == Tool_RectangularSelect) {
            m_selectionPath->Complete();
            m_selectionMask = m_selectionPath->GetMask();
        } else {
            // Nothing -- Free form selection usage is click-move-click.
        }
        break;
    case ToolType_Text:
        // FIXME
        break;
    default:
        wxASSERT(0);
    }
}


void XSCanvas::ScrollBy(int dx, int dy)
{
    int x, y;

    CalcUnscrolledPosition(0, 0, &x, &y);
    Scroll(x + dx, y + dy);
}


void XSCanvas::MoveCursorBy(int dx, int dy)
{
    XSDocument const* doc = m_docchild->GetDocument();

    if (cursorX + dx >= 0 && cursorY + dy >= 0 && cursorX + dx < doc->SquaresX()
            && cursorY + dy < doc->SquaresY()) {
        RefreshSquare(cursorX, cursorY);
        cursorX += dx;
        cursorY += dy;
        RefreshSquare(cursorX, cursorY);  // to draw the cursor
        m_mouseControl = false;
    }
}


void XSCanvas::OnKeyDown(wxKeyEvent& event)
{
    m_docchild->OnKeyDown(event);
}


void XSCanvas::DrawPatternSquare(wxDC& dc, XSSquareIO& square, float zx, float zy, unsigned int z,
        XSFlossPalette const& flossPalette)
{
#if 0
    static struct DrawPatternData {
        float scale;
        float sx;
        float sy;
        bool line;
        float lx1;
        float ly1;
        float lx2;
        float ly2;
    } drawPatternData[] =
    {
        { 0,    0,      0,   false, 0,   0,   0,   0   }, // StitchLoc_None
        { 1.0,  0,      0,   false, 0,   0,   0,   0   }, // StitchLoc_Full
        { 0.75, 0,      0,   true,  .25, 1.0, 1.0, .25 }, // StitchLoc_ThreeQuarterUL
        { 0.75, 1,      0,   true,  0,   .25, .75, 1.0 }, // StitchLoc_ThreeQuarterUR
        { 0.75, 0,      1,   true,  .25, 0,   1.0, .75 }, // StitchLoc_ThreeQuarterLL
        { 0.75, 1,      1,   true,  0,   .75, .75, 0   }, // StitchLoc_ThreeQuarterLR
        { 1.0,  0,      0,   false, 0,   0,   0,   0   }, // StitchLoc_HalfBottom
        { 1.0,  0,      0,   false, 0,   0,   0,   0   }, // StitchLoc_HalfTop
        { 0.5,  0,      0,   true,  0,   .75, .75, 0   }, // StitchLoc_QuarterUL
        { 0.5,  1,      0,   true,  .25, 0,   1.0, .75 }, // StitchLoc_QuarterUR
        { 0.5,  0,      1,   true,  0,   .25, .75, 1.0 }, // StitchLoc_QuarterLL
        { 0.5,  1,      1,   true,  .25, 1.0, 1.0, .25 }, // StitchLoc_QuarterLR
    };

    for (unsigned int i = 0; i < square.stitches; ++i) {
        XSFloss const &floss = flossPalette[square.stitch[i].flossIndex];
        struct DrawPatternData *data = &drawPatternData[square.stitch[i].stitchType];
        float zoom = (float)z;
        float szoom = zoom * data->scale;

        // FIXME:  Move this setup to a PrepDrawPatternSquare()
        wxFont font(be_plain_font);
        font.SetSize(szoom);
        font.SetSpacing(B_CHAR_SPACING);
        SetFont(&font);
        SetHighColor(black);

        // FIXME:  doesn't currently handle multi-byte UTF8
        // FIXME:  slow.  cache this in the XSFloss (?)
        wxChar *symbol = floss.m_symbol;
        wxRect bounding_box;
#if 0
        font.GetBoundingBoxesAsGlyphs(&(symbol[0]), 1, B_SCREEN_METRIC, &bounding_box);
#else
        struct escapement_delta ed = { 0, 0 };
        font.GetBoundingBoxesForStrings(&symbol, 1, B_SCREEN_METRIC, &ed, &bounding_box);
#endif
        float dx, dy;
        dx = ((szoom - bounding_box.right) - bounding_box.left) / 2.0;
        dy = ((szoom - bounding_box.bottom) - bounding_box.top) / 2.0;

#if 0
        DrawChar(symbol[0], wxPoint(zx + (data->sx * (1.0 - data->scale) * zoom) + dx,
                        zy + (data->sy * (1.0 - data->scale) * zoom) + dy));
#else
        DrawString(symbol, wxPoint(zx + (data->sx * (1.0 - data->scale) * zoom) + dx,
                        zy + (data->sy * (1.0 - data->scale) * zoom) + dy));
#endif
        if (data->line) {
            dc.SetPen(gray);
            dc.DrawLine(zx + zoom * data->lx1, zy + zoom * data->ly1,
                    zx + zoom * data->lx2, zy + zoom * data->ly2);
        }
    }
#endif
}


void XSCanvas::DrawRealisticSquare(wxDC& dc, XSSquareIO& /*square*/, float /*zx*/, float /*zy*/,
        unsigned int /*zoom*/, XSFlossPalette const& /*flossPalette*/)
{
    // FIXME
}


void XSCanvas::DrawDesignSquare(wxDC& dc, XSSquareIO& square, float zx, float zy, unsigned int zoom,
        XSFlossPalette const& flossPalette)
{
    const float line_width = 0.2;
    const float li /*line_inset*/ = line_width * 0.375;
    static struct DrawDesignData {
        bool line;
        float xs;
        float ys;
        float xe;
        float ye;
    } drawDesignData[] =
    {
        { true,  li,       li,       1.0 - li, 1.0 - li },
        { true,  li,       1.0 - li, 1.0 - li, li       },  // StitchLoc_Full
        { true,  li,       1.0 - li, 1.0 - li, li       },
        { true,  li,       li,       0.5,      0.5      },  // StitchLoc_ThreeQuarterUL
        { true,  li,       li,       1.0 - li, 1.0 - li },
        { true,  1.0 - li, li,       0.5,      0.5      },  // StitchLoc_ThreeQuarterUR
        { true,  li,       li,       1.0 - li, 1.0 - li },
        { true,  li,       1.0 - li, 0.5,      0.5      },  // StitchLoc_ThreeQuarterLL
        { true,  li,       1.0 - li, 1.0 - li, li       },
        { true,  1.0 - li, 1.0 - li, 0.5,      0.5      },  // StitchLoc_ThreeQuarterLR
        { true,  li,       1.0 - li, 1.0 - li, li       },
        { false, 0,        0,        0,        0        },  // StitchLoc_HalfBottom
        { true,  li,       li,       1.0 - li, 1.0 - li },
        { false, 0,        0,        0,        0        },  // StitchLoc_HalfTop
        { true,  li,       li,       0.3,      0.3      },
        { false, 0,        0,        0,        0        },  // StitchLoc_QuarterUL
        { true,  1.0 - li, li,       0.7,      0.3      },
        { false, 0,        0,        0,        0        },  // StitchLoc_QuarterUR
        { true,  li,       1.0 - li, 0.3,      0.7      },
        { false, 0,        0,        0,        0        },  // StitchLoc_QuarterLL
        { true,  1.0 - li, 1.0 - li, 0.7,      0.7      },
        { false, 0,        0,        0,        0        },  // StitchLoc_QuarterLR
    };

    float pen_width = zoom * line_width;

    for (unsigned int i = 0; i < square.stitches; ++i) {
        struct DrawDesignData* data = &drawDesignData[(square.stitch[i].stitchType - 1) * 2];

        if (data->line) {
            wxColour color = flossPalette[square.stitch[i].flossIndex].GetColor();
            dc.SetPen(wxPen(color, pen_width));
            dc.DrawLine(zx + zoom * data->xs, zy + zoom * data->ys, zx + zoom * data->xe,
                    zy + zoom * data->ye);
            if ((++data)->line)
                dc.DrawLine(zx + zoom * data->xs, zy + zoom * data->ys, zx + zoom * data->xe,
                        zy + zoom * data->ye);
        }
    }

    for (unsigned int i = 0; i < square.knots; ++i) {
        XSFloss const& floss = flossPalette[square.knot[i].flossIndex];
        dc.SetPen(wxPen(floss.GetColor(), pen_width));

        struct RegionOffsetDef* ro = &RegionOffsets[square.knot[i].region];
        switch (square.knot[i].knotType) {
        case Knot_French:
            dc.DrawEllipse(zx + (zoom * ro->xOffset) / 4, zy + (zoom * ro->yOffset) / 4, li, li);
            break;
        default:
            wxASSERT(0);
        }
    }

    for (unsigned int i = 0; i < square.beads; ++i) {
        struct RegionOffsetDef* ro = &RegionOffsets[square.bead[i].region];
        wxColour color;  // = flossPalette->GetColorAtIndex(square.bead[i].colorIndex); // FIXME
                         // beads are color, not floss
        dc.SetPen(wxPen(color, pen_width));
        dc.SetBrush(color);
        dc.DrawEllipse(zx + (zoom * ro->xOffset) / 4, zy + (zoom * ro->yOffset) / 4, li, li);
    }
}


void XSCanvas::OnDraw(wxDC& dc)
{
    XSDocument const* doc = m_docchild->GetDocument();
    unsigned int zoom = doc->GetZoom();
    unsigned int squaresX = doc->SquaresX();
    unsigned int squaresY = doc->SquaresY();

    unsigned int left, top, right, bottom;
    wxCoord x, y, width, height;

    dc.GetClippingBox(&x, &y, &width, &height);
    left = x;
    top = y;
    right = x + width;
    bottom = y + height;
    wxRect updateRect(x, y, width, height);

    /*
     * Refresh the matte background
     */
    dc.SetPen(gray);
    dc.SetBrush(wxBrush(gray));
#if 0
    wxRect rightDeadZone(squaresX * zoom, 0, 100, 100);  // FIXME
    if (rightDeadZone.Intersects(updateRect))
        dc.DrawRectangle(rightDeadZone);
    wxRect bottomDeadZone(0, squaresY * zoom, squaresX * zoom, 100);  // FIXME
    if (bottomDeadZone.Intersects(updateRect))
        dc.DrawRectangle(bottomDeadZone);
#endif

    if (!squaresX || !squaresY)
        return;

    left /= zoom;
    if (left >= squaresX)
        left = squaresX - 1;
    top /= zoom;
    if (top >= squaresY)
        top = squaresY - 1;
    right += (zoom - 1);
    right /= zoom;
    if (right >= squaresX)
        right = squaresX - 1;
    bottom += (zoom - 1);
    bottom /= zoom;
    if (bottom >= squaresY)
        bottom = squaresY - 1;

    dc.SetBrush(wxBrush(cloth_color));
    dc.DrawRectangle(wxRect(left * zoom, top * zoom, (right + 1) * zoom, (bottom + 1) * zoom));

    if (doc->IsShowGrid()) {
        // Draw the gray lines
        for (unsigned int x = left; x <= right; ++x) {
            if (x % 10 == 0)
                continue;
            dc.DrawLine(x * zoom, top * zoom, x * zoom, (bottom + 1) * zoom);
        }
        for (unsigned int y = top; y <= bottom; ++y) {
            if (y % 10 == 0)
                continue;
            dc.DrawLine(left * zoom, y * zoom, (right + 1) * zoom, y * zoom);
        }

        // Now draw the darker black lines every 10 lines.
        // Don't combine this with the above loops because these
        // must be on top (i.e., drawn last).
        dc.SetPen(dark_gray);
        unsigned int top10 = top;
        unsigned int bottom10 = (bottom + 1);
        unsigned int left10 = left;
        unsigned int right10 = (right + 1);
        top10 -= (top10 % 10);
        bottom10 -= (bottom10 % 10);
        left10 -= (left10 % 10);
        right10 -= (right10 % 10);
        for (unsigned int x = left10; x <= right10; x += 10) {
            dc.DrawLine(x * zoom, top * zoom, x * zoom, (bottom + 1) * zoom);
        }
        for (unsigned int y = top10; y <= bottom10; y += 10) {
            dc.DrawLine(left * zoom, y * zoom, (right + 1) * zoom, y * zoom);
        }
    }

    XSFlossPalette const& flossPalette = doc->GetFlossPalette();
    unsigned int layer = doc->GetCurrentLayerIndex();
    enum DrawStyle drawStyle = doc->GetDrawStyle();

    for (unsigned int x = left; x <= right; ++x) {
        for (unsigned int y = top; y <= bottom; ++y) {
            XSSquareIO square;
            float zx = x * zoom;
            float zy = y * zoom;

            doc->GetSquareData(&square, x, y, layer);
            if (drawStyle == DrawStyle_Design) {
                DrawDesignSquare(dc, square, zx, zy, zoom, flossPalette);
            } else if (drawStyle == DrawStyle_Pattern) {
                DrawPatternSquare(dc, square, zx, zy, zoom, flossPalette);
            } else {
                wxASSERT(drawStyle == DrawStyle_Realistic);
                DrawRealisticSquare(dc, square, zx, zy, zoom, flossPalette);
            }
        }
    }

    if (m_resetCursor || SquareInnerRect(cursorX, cursorY).Intersects(updateRect)) {
        m_resetCursor = false;

#if 0
        unsigned int x, y;
        if (m_mouseControl) {
            wxPoint cursor = wxGetMousePosition();
            x = ((unsigned int)(cursor.x / zoom));
            y = ((unsigned int)(cursor.y / zoom));
            if (x < squaresX && y < squaresY) {
                if (x != cursorX || y != cursorY)
                    RefreshSquare(cursorX, cursorY);
                cursorX = x;
                cursorY = y;
            }
        }
#endif
        dc.SetPen(cursor_color);
        // FIXME: change to:  dc.DrawRectangle(SquareInnerRect());
        dc.DrawLine(cursorX * zoom + 1, cursorY * zoom + 1, cursorX * zoom + zoom - 1,
                cursorY * zoom + 1);
        dc.DrawLine(cursorX * zoom + 1, (cursorY + 1) * zoom - 1, cursorX * zoom + zoom - 1,
                (cursorY + 1) * zoom - 1);
        dc.DrawLine(cursorX * zoom + 1, cursorY * zoom + 1, cursorX * zoom + 1,
                (cursorY + 1) * zoom - 1);
        dc.DrawLine((cursorX + 1) * zoom - 1, cursorY * zoom + 1, (cursorX + 1) * zoom - 1,
                (cursorY + 1) * zoom - 1);
    }

    if (m_docchild->GetToolType() == ToolType_Select && m_selectionMask) {
        // FIXME -- draw current path; shade selection
    }
}


void XSCanvas::AdjustScrollBars()
{
    unsigned int zoom = m_docchild->GetDocument()->GetZoom();

    if (!zoom)
        // Document is not yet fully initialized.
        return;

    unsigned int squaresX = m_docchild->GetDocument()->SquaresX();
    unsigned int squaresY = m_docchild->GetDocument()->SquaresY();

    float docWidth = zoom * squaresX;
    float docHeight = zoom * squaresY;

    SetVirtualSize(docWidth, docHeight);
    SetScrollRate(zoom, zoom);
    SetScrollPageSize(zoom * 10, zoom * 10);
    m_resetCursor = true;
}
