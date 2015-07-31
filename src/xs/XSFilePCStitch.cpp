#include "XSController.h"
#include "XSFile.h"
#include "XSFilePCStitch.h"
#include "XSSquareIO.h"


XSFileOutPCStitch::XSFileOutPCStitch(std::ostream &file,
        ProgressFn *progressFn,
        void *handle) :
    XSFileOut(file, progressFn, handle)
{
}

XSFileOutPCStitch::~XSFileOutPCStitch()
{
}

void XSFileOutPCStitch::Write(XSController * /*document*/)
{
    // FIXME
}

XSFileInPCStitch::XSFileInPCStitch(std::istream &file,
        ProgressFn *progressFn,
        void *handle) :
    XSFileIn(file, progressFn, handle)
{
}

XSFileInPCStitch::~XSFileInPCStitch()
{
}

XSController *XSFileInPCStitch::Read()
{
    // FIXME
    return 0;
}

bool XSFileInPCStitch::IsHandled(std::istream & /*file*/)
{
    // FIXME
    return false;
}
