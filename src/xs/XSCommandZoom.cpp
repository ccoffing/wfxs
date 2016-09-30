#include "XSCommandZoom.h"
#include "XSModel.h"


XSCommandZoom::XSCommandZoom(unsigned int zoom)
    : m_oldZoom(zoom /* meaningless, to pacify compiler*/)
    , m_newZoom(zoom)
{
}

int XSCommandZoom::doCommand()
{
    m_oldZoom = m_model->getZoom();
    m_model->setZoom(m_newZoom);
    return true;
}

int XSCommandZoom::undoCommand()
{
    m_model->setZoom(m_oldZoom);
    return true;
}

char const* XSCommandZoom::getDescription() const
{
    return _("zoom");
}
