#include "XSCommandSetSize.h"
#include "XSModel.h"


XSCommandSetSize::XSCommandSetSize(unsigned int x, unsigned int y)
    : m_newX(x)
    , m_newY(y)
    , m_oldX(0)
    , m_oldY(0)
{
}

int XSCommandSetSize::doCommand()
{
    m_oldX = m_model->squaresX();
    m_oldY = m_model->squaresY();
    // FIXME:  save the stitches
    m_model->resize(m_newX, m_newY);
    return true;
}

int XSCommandSetSize::undoCommand()
{
    m_model->resize(m_oldX, m_oldY);
    // FIXME:  restore the stitches
    return true;
}

char const* XSCommandSetSize::getDescription() const
{
    return _("resize");
}
