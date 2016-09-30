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

int XSCommandSetStitch::doCommand()
{
    m_layer = m_model->getCurrentLayerIndex();
    m_model->getSquareData(&m_oldSquare, m_x, m_y, m_layer);
    m_stitchType = m_model->setStitch(
            m_x, m_y, m_xPercent, m_yPercent, m_stitchType, m_flossIndex, m_overwrite);
    return true;
}

int XSCommandSetStitch::undoCommand()
{
    m_model->setSquareData(&m_oldSquare, m_x, m_y, m_layer);
    return true;
}

char const* XSCommandSetStitch::getDescription() const
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

int XSCommandSetStitches::doCommand()
{
    m_layer = m_model->getCurrentLayerIndex();
    for (auto& elem : m_points) {
        XSSquareIO square;
        m_model->getSquareData(&square, elem.x, elem.y, m_layer);
        m_oldSquares.insert(m_oldSquares.end(), square);
    }
    // FIXME:  SetStitch should take m_layer!!!!!!!!!!!
    for (auto& elem : m_points) {
        m_stitchType = m_model->setStitch(
                elem.x, elem.y, 50, 50, m_stitchType, m_flossIndex, m_overwrite);
    }
    return true;
}

int XSCommandSetStitches::undoCommand()
{
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_model->setSquareData(&m_oldSquares[i], m_points[i].x, m_points[i].y, m_layer);
    }
    return true;
}

char const* XSCommandSetStitches::getDescription() const
{
    return _("stitches");
}
