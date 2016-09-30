#include "XSCommandSetProperties.h"
#include "XSModel.h"
#include "XSProperties.h"


XSCommandSetProperties::XSCommandSetProperties(XSProperties const& properties)
    : m_oldProperties()
    , m_newProperties(properties)
{
}

int XSCommandSetProperties::doCommand()
{
    m_oldProperties = m_model->getProperties();
    m_model->setProperties(m_newProperties);
    return true;
}

int XSCommandSetProperties::undoCommand()
{
    m_model->setProperties(m_oldProperties);
    return true;
}

char const* XSCommandSetProperties::getDescription() const
{
    return _("document properties");
}
