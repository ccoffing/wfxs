#include "XSCommandSetStyle.h"
#include "XSModel.h"


XSCommandSetStyle::XSCommandSetStyle(enum DrawStyle style)
    : m_newStyle(style)
    , m_oldStyle(style)
{
}

int XSCommandSetStyle::Do()
{
    m_oldStyle = m_model->GetDrawStyle();
    m_model->SetDrawStyle(m_newStyle);
    return true;
}

int XSCommandSetStyle::Undo()
{
    m_model->SetDrawStyle(m_oldStyle);
    return true;
}

char const* XSCommandSetStyle::GetDescription() const
{
    return _("set style");
}
