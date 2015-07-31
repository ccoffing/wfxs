#include "XSDataIO.h"
#include "XSFile.h"

#include "clc/support/Debug.h"


XSFileIn::XSFileIn(std::istream &file,
        ProgressFn *progressFn,
        void *handle) :
    m_file(file),
    m_progressFn(progressFn),
    m_handle(handle)
{
}

XSFileIn::~XSFileIn()
{
}

XSFileOut::XSFileOut(std::ostream &file,
        ProgressFn *progressFn,
        void *handle) :
    m_file(file),
    m_progressFn(progressFn),
    m_handle(handle)
{
}

XSFileOut::~XSFileOut()
{
}
