#include "XSCommandSetTool.h"
#include "XSModel.h"


XSCommandSetTool::XSCommandSetTool()
{
}

int XSCommandSetTool::doCommand()
{
    return true;
}

int XSCommandSetTool::undoCommand()
{
    return true;
}

char const* XSCommandSetTool::getDescription() const
{
    return _("");
}
