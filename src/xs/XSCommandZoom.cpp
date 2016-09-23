#include "XSCommandZoom.h"
#include "XSModel.h"


XSCommandZoom::XSCommandZoom(unsigned int zoom)
    : m_oldZoom(zoom /* meaningless, to pacify compiler*/)
    , m_newZoom(zoom)
{
}

int XSCommandZoom::Do()
{
    m_oldZoom = m_model->GetZoom();
    m_model->SetZoom(m_newZoom);
    return true;
}

int XSCommandZoom::Undo()
{
    m_model->SetZoom(m_oldZoom);
    return true;
}

char const* XSCommandZoom::GetDescription() const
{
    return _("zoom");
}
