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

int XSCommandClearSquare::Do()
{
    m_model->GetSquareData(&m_oldSquare, m_x, m_y, m_layer);
    m_model->ClearSquare(m_x, m_y, m_layer);
    return true;
}

int XSCommandClearSquare::Undo()
{
    m_model->SetSquareData(&m_oldSquare, m_x, m_y, m_layer);
    return true;
}

char const* XSCommandClearSquare::GetDescription() const
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

int XSCommandClearSquares::Do()
{
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        XSSquareIO square;
        m_model->GetSquareData(&square, m_points[i].x, m_points[i].y, m_layer);
        m_oldSquares.push_back(square);
    }
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_model->ClearSquare(m_points[i].x, m_points[i].y, m_layer);
    }
    return true;
}

int XSCommandClearSquares::Undo()
{
    for (unsigned int i = 0; i < m_points.size(); ++i) {
        m_model->SetSquareData(&m_oldSquares[i], m_points[i].x, m_points[i].y, m_layer);
    }
    return true;
}

char const* XSCommandClearSquares::GetDescription() const
{
    return _("erase squares");
}
