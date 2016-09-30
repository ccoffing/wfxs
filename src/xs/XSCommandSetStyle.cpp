#include "XSCommandSetStyle.h"
#include "XSModel.h"


XSCommandSetStyle::XSCommandSetStyle(enum DrawStyle style)
    : m_newStyle(style)
    , m_oldStyle(style)
{
}

int XSCommandSetStyle::doCommand()
{
    m_oldStyle = m_model->getDrawStyle();
    m_model->setDrawStyle(m_newStyle);
    return true;
}

int XSCommandSetStyle::undoCommand()
{
    m_model->setDrawStyle(m_oldStyle);
    return true;
}

char const* XSCommandSetStyle::getDescription() const
{
    return _("set style");
}
