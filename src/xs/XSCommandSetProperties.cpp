#include "XSCommandSetProperties.h"
#include "XSModel.h"
#include "XSProperties.h"


XSCommandSetProperties::XSCommandSetProperties(XSProperties const& properties)
    : m_oldProperties()
    , m_newProperties(properties)
{
}

int XSCommandSetProperties::Do()
{
    m_oldProperties = m_model->GetProperties();
    m_model->SetProperties(m_newProperties);
    return true;
}

int XSCommandSetProperties::Undo()
{
    m_model->SetProperties(m_oldProperties);
    return true;
}

char const* XSCommandSetProperties::GetDescription() const
{
    return _("document properties");
}
