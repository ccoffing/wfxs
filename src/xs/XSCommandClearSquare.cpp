#include "XSCommandClearSquare.h"
#include "XSModel.h"
#include "XSPoint.h"

#include <vector>


XSCommandClearSquare::XSCommandClearSquare(unsigned int x, unsigned int y, unsigned int layer)
    : m_layer(layer)
    , m_x(x)
    , m_y(y)
    , m_oldSquare()
{
}

int XSCommandClearSquare::doCommand()
{
    m_model->getSquareData(&m_oldSquare, m_x, m_y, m_layer);
    m_model->clearSquare(m_x, m_y, m_layer);
    return true;
}

int XSCommandClearSquare::undoCommand()
{
    m_model->setSquareData(&m_oldSquare, m_x, m_y, m_layer);
    return true;
}

char const* XSCommandClearSquare::getDescription() const
{
    return _("erase square");
}

XSCommandClearSquares::XSCommandClearSquares(std::vector<XSPoint> const& points, unsigned int layer)
    : m_layer(layer)
    , m_points(points)
    , m_oldSquares()
{
    m_oldSquares.reserve(points.size());
}

int XSCommandClearSquares::doCommand()
{
    for (auto const& point : m_points) {
        XSSquareIO square;
        m_model->getSquareData(&square, point.x, point.y, m_layer);
        m_oldSquares.push_back(square);
    }
    for (auto const& point : m_points) {
        m_model->clearSquare(point.x, point.y, m_layer);
    }
    return true;
}

int XSCommandClearSquares::undoCommand()
{
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_model->setSquareData(&m_oldSquares[i], m_points[i].x, m_points[i].y, m_layer);
    }
    return true;
}

char const* XSCommandClearSquares::getDescription() const
{
    return _("erase squares");
}
