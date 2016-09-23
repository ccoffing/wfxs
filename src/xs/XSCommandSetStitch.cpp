#include "XSCommandSetStitch.h"
#include "XSModel.h"
#include "XSPoint.h"
#include "XSSquareIO.h"


XSCommandSetStitch::XSCommandSetStitch(unsigned int x, unsigned int y, unsigned int xPercent,
        unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite)
    : m_layer((unsigned int)-1)
    , m_x(x)
    , m_y(y)
    , m_xPercent(xPercent)
    , m_yPercent(yPercent)
    , m_stitchType(stitchType)
    , m_flossIndex(flossIndex)
    , m_overwrite(overwrite)
    , m_oldSquare()
{
}

int XSCommandSetStitch::Do()
{
    m_layer = m_model->GetCurrentLayerIndex();
    m_model->GetSquareData(&m_oldSquare, m_x, m_y, m_layer);
    m_stitchType = m_model->SetStitch(
            m_x, m_y, m_xPercent, m_yPercent, m_stitchType, m_flossIndex, m_overwrite);
    return true;
}

int XSCommandSetStitch::Undo()
{
    m_model->SetSquareData(&m_oldSquare, m_x, m_y, m_layer);
    return true;
}

char const* XSCommandSetStitch::GetDescription() const
{
    return _("stitch");
}

XSCommandSetStitches::XSCommandSetStitches(std::vector<XSPoint>& points, StitchType stitchType,
        unsigned int flossIndex, bool overwrite)
    : m_layer((unsigned int)-1)
    , m_points(points)
    , m_stitchType(stitchType)
    , m_flossIndex(flossIndex)
    , m_overwrite(overwrite)
    , m_oldSquares()
{
    m_oldSquares.reserve(points.size());
}

int XSCommandSetStitches::Do()
{
    m_layer = m_model->GetCurrentLayerIndex();
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        XSSquareIO square;
        m_model->GetSquareData(&square, m_points[i].x, m_points[i].y, m_layer);
        m_oldSquares.insert(m_oldSquares.end(), square);
    }
    // FIXME:  SetStitch should take m_layer!!!!!!!!!!!
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_stitchType = m_model->SetStitch(
                m_points[i].x, m_points[i].y, 50, 50, m_stitchType, m_flossIndex, m_overwrite);
    }
    return true;
}

int XSCommandSetStitches::Undo()
{
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_model->SetSquareData(&m_oldSquares[i], m_points[i].x, m_points[i].y, m_layer);
    }
    return true;
}

char const* XSCommandSetStitches::GetDescription() const
{
    return _("stitches");
}
