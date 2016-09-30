#include "XSCommandSetKnot.h"
#include "XSModel.h"


XSCommandSetKnot::XSCommandSetKnot(unsigned int x, unsigned int y, unsigned int region,
        KnotType knotType, unsigned int flossIndex, bool overwrite)
    : m_layer((unsigned int)-1)
    , m_x(x)
    , m_y(y)
    , m_region(region)
    , m_knotType(knotType)
    , m_flossIndex(flossIndex)
    , m_overwrite(overwrite)
{
}

int XSCommandSetKnot::doCommand()
{
    m_layer = m_model->getCurrentLayerIndex();
    // FIXME  save old knot/bead at this location
    m_model->setKnot(m_x, m_y, m_region, m_knotType, m_flossIndex, m_overwrite);
    return true;
}

int XSCommandSetKnot::undoCommand()
{
    // m_model->setSquareData(m_layer, m_x, m_y, &m_oldSquare);
    return true;
}

char const* XSCommandSetKnot::getDescription() const
{
    return _("knot");
}
