#include "qt/XSQtCanvas.h"

#include "xs/BresenhamLine.h"
#include "xs/XSColor.h"
#include "xs/XSFloss.h"
#include "xs/XSSelection.h"
#include "xs/XSSelectionPath.h"

#include <QtGui>

#include <assert.h>


/**
 * Here is the subdivision for embelishments (knots, beads, etc).  Note that "R" means the spot is
 * handled by the square to the right; "B" means the square below, and "D" means the diagonal
 * square.
 *
 *    0---1---R
 *    | 3   4 |
 *    5   6   R
 *    | 8   9 |
 *    B---B---D
 *
 * Backstitches, also, can start or end at any of these locations.
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

    if (intercept1 < 25) {
        // First lower-left-to-upper-right diagonal
        r = 0;
    } else if (intercept1 < 75) {
        // Second diagonal
        if (intercept2 > 25)
            r = 1;
        else if (intercept2 > -25)
            r = 3;
        else
            r = 5;
    } else if (intercept1 < 125) {
        // Third diagonal
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
    } else if (intercept1 < 175) {
        // Forth diagonal
        if (intercept2 > 25)
            r = 7;
        else if (intercept2 > -25)
            r = 9;
        else
            r = 11;
    } else {
        // Fifth diagonal
        r = 12;
    }
    return r;
}

static bool DeJiggle(unsigned int x, unsigned int y, bool reset)
{
    /* I'm playing a little fast-and-loose by having these global-static (shared between instances)
     * but you'll never be able to drag from one window to the same square on another (there will be
     * a MouseDown event, which will reset these).
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

XSQtCanvas::XSQtCanvas(XSModel& model, XSController& controller, QWidget* parent)
    : QWidget(parent)
    , m_model(model)
    , m_controller(controller)
    , cursorX(0)
    , cursorY(0)
    , m_resetCursor(true)
    , m_mouseDown(false)
    , m_cmdDown(false)
    , m_prevX((unsigned int)-1)
    , m_prevY((unsigned int)-1)
    , m_prevRegion((unsigned int)-1)
    , m_selectionPath(0)
    , m_selectionMask(0)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize XSQtCanvas::minimumSizeHint() const
{
    unsigned int squaresX = m_model.SquaresX();
    unsigned int squaresY = m_model.SquaresY();

    if (squaresX > 10)
        squaresX = 10;
    if (squaresY > 10)
        squaresY = 10;
    unsigned int zoom = m_model.GetZoom();
    return QSize(squaresX * zoom + 1, squaresY * zoom + 1);
}

QSize XSQtCanvas::sizeHint() const
{
    unsigned int squaresX = m_model.SquaresX();
    unsigned int squaresY = m_model.SquaresY();
    unsigned int zoom = m_model.GetZoom();

    return QSize(squaresX * zoom + 1, squaresY * zoom + 1);
}

QRect XSQtCanvas::squareInnerRect(unsigned int x, unsigned int y)
{
    unsigned int zoom = m_model.GetZoom();

    return QRect(x * zoom + 1, y * zoom + 1, x * zoom + zoom - 1, y * zoom + zoom - 1);
}

void XSQtCanvas::refreshSquare(unsigned int squareX, unsigned int squareY)
{
    update(squareInnerRect(squareX, squareY));
}

void XSQtCanvas::mouseReleaseEvent(QMouseEvent* event)
{
    m_mouseDown = false;
}

void XSQtCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    m_mouseDown = true;

    unsigned int zoom = m_model.GetZoom();
    int squareX = event->x() / zoom;
    int squareY = event->y() / zoom;

    m_prevX = squareX;
    m_prevY = squareY;

    XSToolState const toolState = m_model.ToolState();
    switch (toolState.m_toolType) {
    case ToolType_Stitch:
        DeJiggle(squareX, squareY, true);
        if (m_cmdDown)
            m_controller.OnClearSquare(squareX, squareY);
        else
            // FIXME:  pass region instead
            m_controller.OnSetStitch(squareX, squareY, 50, 50);  // xPercent, yPercent);
        break;
    case ToolType_Bead: {
        DeJiggle(squareX, squareY, true);
        // struct RegionOffsetDef* ro = &RegionOffsets[region];
        // m_controller.OnSetBead(squareX + ro->newX, squareY + ro->newY, ro->newRegion);
        break;
    }
    case ToolType_Knot: {
        DeJiggle(squareX, squareY, true);
        // struct RegionOffsetDef* ro = & RegionOffsets[region];
        // m_controller.OnSetKnot(squareX + ro->newX, squareY + ro->newY, ro->newRegion);
        break;
    }
    case ToolType_Eraser: {
        DeJiggle(squareX, squareY, true);
        m_controller.OnClearSquare(squareX, squareY);
        break;
    }
    case ToolType_FloodFill: {
        XSSquareIO oldSquare;
        m_model.GetSquareData(&oldSquare, squareX, squareY, m_model.GetCurrentLayerIndex());
#if 0
        // zzz   seems like I need a CalculateSquare, similar to setSquare, but
        //      it directly calls the doc functions, saves the resulting square,
        //      and then undoes it.  Yuck
        XSSquareIO newSquare = doc->CalculateSquare(squareX, squareY,
                ((unsigned int)point.x) % zoom * 100 / zoom,
                ((unsigned int)point.y) % zoom * 100 / zoom,
                toolState.m_toolType, true, false);

        m_controller.CommandFloodFill(squareX, squareY, oldSquare, newSquare);
#endif
        break;
    }
    case ToolType_Select: {
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
    }
    case ToolType_ColorPicker: {
        XSSquareIO square;
        m_model.GetSquareData(&square, squareX, squareY, m_model.GetCurrentLayerIndex());
        //            if (square.num_beads
        // FIXME ... look at region; pick relevant stich
        break;
    }
    case ToolType_Backstitch: {
        // FIXME
        break;
    }
    case ToolType_Couching: {
        // FIXME
        break;
    }
    case ToolType_Text: {
        // FIXME
        break;
    }
    default:
        assert(0);
    }

    refreshSquare(squareX, squareY);
}

void XSQtCanvas::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_mouseDown)
        return;

    unsigned int zoom = m_model.GetZoom();
    int squareX = event->x() / zoom;
    int squareY = event->y() / zoom;

    if (squareX < 0 || (unsigned int)squareX >= m_model.SquaresX() || squareY < 0
            || (unsigned int)squareY >= m_model.SquaresX())
        return;

    if (!DeJiggle(squareX, squareY, false))
        return;

    switch (m_model.m_toolState.m_toolType) {
    case ToolType_Stitch: {
        std::vector<XSPoint> points;
        BresenhamLine<XSPoint>(m_prevX, m_prevY, squareX, squareY, points);
        // First point has already been stitched by MouseDown
        assert(points.size() > 1);
        points.erase(points.begin());

        if (m_cmdDown)
            m_controller.OnClearSquares(points);
        else
            m_controller.OnSetStitches(points);

        for (auto& point : points)
            refreshSquare(point.x, point.y);

        m_prevX = squareX;
        m_prevY = squareY;
        break;
    }
    default:
        // FIXME:  Handle other types, as appropriate
        break;
    }
}

void XSQtCanvas::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);

    QPen blackPen(Qt::black, 1);
    QPen grayPen(Qt::gray, 1);

#if 0
    // Refresh the matte background
    QBrush brush(Qt::Dense3Pattern);
    brush.setColor(Qt::gray);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width(), height());
#endif

    unsigned int squaresX = m_model.SquaresX();
    unsigned int squaresY = m_model.SquaresY();

    unsigned int zoom = m_model.GetZoom();
    unsigned int left = 0;
    unsigned int top = 0;
    unsigned int right = left + width();
    unsigned int bottom = top + height();

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

    QBrush brush(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.setPen(blackPen);
    painter.drawRect(0, 0, squaresX * zoom, squaresY * zoom);

    if (m_model.IsShowGrid()) {
        painter.setPen(grayPen);
        // Draw the gray lines
        for (unsigned int x = left; x <= right; ++x) {
            if (x % 10 == 0)
                continue;
            painter.drawLine(x * zoom, top * zoom, x * zoom, (bottom + 1) * zoom);
        }
        for (unsigned int y = top; y <= bottom; ++y) {
            if (y % 10 == 0)
                continue;
            painter.drawLine(left * zoom, y * zoom, (right + 1) * zoom, y * zoom);
        }

        // Now draw the darker black lines every 10 lines.
        // Don't combine this with the above loops because these
        // must be on top (i.e., drawn last).
        painter.setPen(blackPen);
        unsigned int top10 = top;
        unsigned int bottom10 = (bottom + 1);
        unsigned int left10 = left;
        unsigned int right10 = (right + 1);
        top10 -= (top10 % 10);
        bottom10 -= (bottom10 % 10);
        left10 -= (left10 % 10);
        right10 -= (right10 % 10);
        for (unsigned int x = left10; x <= right10; x += 10) {
            painter.drawLine(x * zoom, top * zoom, x * zoom, (bottom + 1) * zoom);
        }
        for (unsigned int y = top10; y <= bottom10; y += 10) {
            painter.drawLine(left * zoom, y * zoom, (right + 1) * zoom, y * zoom);
        }
    }

    const XSFlossPalette& flossPalette = m_model.GetFlossPalette();
    unsigned int layer = m_model.GetCurrentLayerIndex();
    enum DrawStyle drawStyle = m_model.GetDrawStyle();

    for (unsigned int x = left; x <= right; ++x) {
        for (unsigned int y = top; y <= bottom; ++y) {
            XSSquareIO square;
            double zx = x * zoom;
            double zy = y * zoom;

            m_model.GetSquareData(&square, x, y, layer);
            if (drawStyle == DrawStyle_Design) {
                DrawDesignSquare(painter, square, zx, zy, zoom, flossPalette);
            } else if (drawStyle == DrawStyle_Pattern) {
                DrawPatternSquare(painter, square, zx, zy, zoom, flossPalette);
            } else {
                assert(drawStyle == DrawStyle_Realistic);
                DrawRealisticSquare(painter, square, zx, zy, zoom, flossPalette);
            }
        }
    }

    if (m_resetCursor) {  // || squareInnerRect(cursorX, cursorY).Intersects(updateRect))
        m_resetCursor = false;

#if 0
        static const QPoint points[4] = {
            QPoint(10, 80),
            QPoint(20, 10),
            QPoint(80, 30),
            QPoint(90, 70)
        };

        QRect rect(10, 20, 80, 60);

        QPainterPath path;
        path.moveTo(20, 80);
        path.lineTo(20, 30);
        path.cubicTo(80, 0, 50, 50, 80, 80);

        int startAngle = 20 * 16;
        int arcLength = 120 * 16;

        QPainter painter(this);
        painter.setPen(pen);
        painter.setBrush(brush);
        if (antialiased)
            painter.setRenderHint(QPainter::Antialiasing, true);

        for (int x = 0; x < width(); x += 100) {
            for (int y = 0; y < height(); y += 100) {
                painter.save();
                painter.translate(x, y);
                if (transformed) {
                    painter.translate(50, 50);
                    painter.rotate(60.0);
                    painter.scale(0.6, 0.9);
                    painter.translate(-50, -50);
                }

                switch (shape) {
                case Line:
                    painter.drawLine(rect.bottomLeft(), rect.topRight());
                    break;
                case Points:
                    painter.drawPoints(points, 4);
                    break;
                case Polyline:
                    painter.drawPolyline(points, 4);
                    break;
                case Polygon:
                    painter.drawPolygon(points, 4);
                    break;
                case Rect:
                    painter.drawRect(rect);
                    break;
                case RoundedRect:
                    painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
                    break;
                case Ellipse:
                    painter.drawEllipse(rect);
                    break;
                case Arc:
                    painter.drawArc(rect, startAngle, arcLength);
                    break;
                case Chord:
                    painter.drawChord(rect, startAngle, arcLength);
                    break;
                case Pie:
                    painter.drawPie(rect, startAngle, arcLength);
                    break;
                case Path:
                    painter.drawPath(path);
                    break;
                case Text:
                    painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nDigia"));
                    break;
//			case Pixmap:
//				painter.drawPixmap(10, 10, pixmap);
                }
                painter.restore();
            }
#endif
    }

#if 0
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
#endif
}

void XSQtCanvas::DrawDesignSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
        unsigned int zoom, XSFlossPalette const& flossPalette)
{
    const double line_width = 0.2;
    const double li /*line_inset*/ = line_width * 0.375;
    static struct DrawDesignData {
        bool line;
        double xs;
        double ys;
        double xe;
        double ye;
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

    QPen pen;

    pen.setWidth(zoom * line_width);

    for (unsigned int i = 0; i < square.stitches; ++i) {
        struct DrawDesignData* data = &drawDesignData[(square.stitch[i].stitchType - 1) * 2];

        if (data->line) {
            XSColor c = flossPalette[square.stitch[i].flossIndex].GetColor();
            QColor color(c.red, c.green, c.blue);
            pen.setColor(color);
            painter.setPen(pen);
            painter.drawLine(zx + zoom * data->xs, zy + zoom * data->ys, zx + zoom * data->xe,
                    zy + zoom * data->ye);
            if ((++data)->line)
                painter.drawLine(zx + zoom * data->xs, zy + zoom * data->ys, zx + zoom * data->xe,
                        zy + zoom * data->ye);
        }
    }

    for (unsigned int i = 0; i < square.knots; ++i) {
        XSColor c = flossPalette[square.knot[i].flossIndex].GetColor();
        QColor color(c.red, c.green, c.blue);
        pen.setColor(color);
        painter.setPen(pen);

        struct RegionOffsetDef* ro = &RegionOffsets[square.knot[i].region];
        switch (square.knot[i].knotType) {
        case Knot_French:
            painter.drawEllipse(
                    zx + (zoom * ro->xOffset) / 4, zy + (zoom * ro->yOffset) / 4, li, li);
            break;
        default:
            assert(0);
        }
    }

    for (unsigned int i = 0; i < square.beads; ++i) {
        struct RegionOffsetDef* ro = &RegionOffsets[square.bead[i].region];
        QColor color;  // = flossPalette->GetColorAtIndex(square.bead[i].colorIndex); // FIXME beads
                       // are color, not floss
        painter.setPen(pen);
        painter.setBrush(color);
        painter.drawEllipse(zx + (zoom * ro->xOffset) / 4, zy + (zoom * ro->yOffset) / 4, li, li);
    }
}

void XSQtCanvas::DrawPatternSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
        unsigned int zoom, XSFlossPalette const& flossPalette)
{
}

void XSQtCanvas::DrawRealisticSquare(QPainter& painter, XSSquareIO& square, double zx, double zy,
        unsigned int zoom, XSFlossPalette const& flossPalette)
{
}
