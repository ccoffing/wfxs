#include "XSCommandSetFlossPalette.h"
#include "XSFlossPalette.h"
#include "XSModel.h"


XSCommandSetFlossPalette::XSCommandSetFlossPalette(XSFlossPalette const& flossPalette)
    : m_oldFlossPalette()
    , m_newFlossPalette(flossPalette)
{
}

int XSCommandSetFlossPalette::doCommand()
{
    m_oldFlossPalette = m_model->getFlossPalette();
    m_model->setFlossPalette(m_newFlossPalette);
    return true;
}

int XSCommandSetFlossPalette::undoCommand()
{
    m_model->setFlossPalette(m_oldFlossPalette);
    return true;
}

char const* XSCommandSetFlossPalette::getDescription() const
{
    return _("floss palette");
}
