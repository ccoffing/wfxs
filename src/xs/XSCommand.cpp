#include "XSCommand.h"
#include "XSController.h"


void XSCommand::init(XSModel* model)
{
    m_model = model;
}

char const* XSCommand::getDescription() const
{
    return "";
}
