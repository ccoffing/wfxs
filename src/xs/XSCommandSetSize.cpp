#include "XSCommandSetSize.h"
#include "XSModel.h"


XSCommandSetSize::XSCommandSetSize(unsigned int x, unsigned int y)
    : m_newX(x)
    , m_newY(y)
    , m_oldX(0)
    , m_oldY(0)
{
}

int XSCommandSetSize::Do()
{
    m_oldX = m_model->SquaresX();
    m_oldY = m_model->SquaresY();
    // FIXME:  save the stitches
    m_model->Resize(m_newX, m_newY);
    return true;
}

int XSCommandSetSize::Undo()
{
    m_model->Resize(m_oldX, m_oldY);
    // FIXME:  restore the stitches
    return true;
}

char const* XSCommandSetSize::GetDescription() const
{
    return _("resize");
}
