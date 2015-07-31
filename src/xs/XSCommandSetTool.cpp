#include "XSCommandSetTool.h"
#include "XSModel.h"


XSCommandSetTool::XSCommandSetTool()
{
}

int XSCommandSetTool::Do()
{
    return true;
}

int XSCommandSetTool::Undo()
{
    return true;
}

char const *XSCommandSetTool::GetDescription() const
{
    return _("");
}
