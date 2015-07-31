#include "XSCommandShowRulers.h"
#include "XSModel.h"


XSCommandShowRulers::XSCommandShowRulers(bool showRulers) :
    m_oldShowRulers(showRulers /* meaninless, to pacify compiler*/),
    m_newShowRulers(showRulers)
{
}

int XSCommandShowRulers::Do()
{
    m_oldShowRulers = m_model->IsShowRulers();
    m_model->SetShowRulers(m_newShowRulers);
    return true;
}

int XSCommandShowRulers::Undo()
{
    m_model->SetShowRulers(m_oldShowRulers);
    return true;
}

char const *XSCommandShowRulers::GetDescription() const
{
    return _("show rulers");
}
