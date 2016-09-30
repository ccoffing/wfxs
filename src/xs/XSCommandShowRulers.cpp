#include "XSCommandShowRulers.h"
#include "XSModel.h"


XSCommandShowRulers::XSCommandShowRulers(bool showRulers)
    : m_oldShowRulers(false)
    , m_newShowRulers(showRulers)
{
}

int XSCommandShowRulers::doCommand()
{
    m_oldShowRulers = m_model->isShowRulers();
    m_model->setShowRulers(m_newShowRulers);
    return true;
}

int XSCommandShowRulers::undoCommand()
{
    m_model->setShowRulers(m_oldShowRulers);
    return true;
}

char const* XSCommandShowRulers::getDescription() const
{
    return _("show rulers");
}
