#include "XSCommand.h"
#include "XSController.h"


void XSCommand::Init(XSModel *model)
{
    m_model = model;
}

char const *XSCommand::GetDescription() const
{
    return "";
}
