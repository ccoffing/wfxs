#include "XSCommandSetBead.h"
#include "XSModel.h"


XSCommandSetBead::XSCommandSetBead(unsigned int x, unsigned int y, unsigned int region,
        unsigned int colorIndex, bool overwrite)
    : m_layer((unsigned int)-1)
    , m_x(x)
    , m_y(y)
    , m_region(region)
    , m_colorIndex(colorIndex)
    , m_overwrite(overwrite)
{
}

int XSCommandSetBead::Do()
{
    m_layer = m_model->GetCurrentLayerIndex();
    // FIXME  save old knot/bead at this location
    m_model->SetBead(m_x, m_y, m_region, m_colorIndex, m_overwrite);
    return true;
}

int XSCommandSetBead::Undo()
{
    // m_model->SetSquareData(m_layer, m_x, m_y, &m_oldSquare);
    return true;
}

char const* XSCommandSetBead::GetDescription() const
{
    return _("bead");
}
