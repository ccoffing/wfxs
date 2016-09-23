#include "XSCommandSetFlossPalette.h"
#include "XSFlossPalette.h"
#include "XSModel.h"


XSCommandSetFlossPalette::XSCommandSetFlossPalette(XSFlossPalette const& flossPalette)
    : m_oldFlossPalette()
    , m_newFlossPalette(flossPalette)
{
}

int XSCommandSetFlossPalette::Do()
{
    m_oldFlossPalette = m_model->GetFlossPalette();
    m_model->SetFlossPalette(m_newFlossPalette);
    return true;
}

int XSCommandSetFlossPalette::Undo()
{
    m_model->SetFlossPalette(m_oldFlossPalette);
    return true;
}

char const* XSCommandSetFlossPalette::GetDescription() const
{
    return _("floss palette");
}
