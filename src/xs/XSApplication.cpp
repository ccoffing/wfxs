#include "XSApplication.h"
#include "XSData.h"

#include "clc/support/LogAppenders.h"
#include "clc/support/Logger.h"


XSApplication *xs_app;
static clc::LogAppenderCFile console(stderr);

XSApplication::XSApplication()
{
#ifdef DEBUG
    clc::Log::get("")->setLevel(clc::Log::Trace);
#else
    clc::Log::get("")->setLevel(clc::Log::Warn);
#endif
    clc::Log::get("")->setAppender(&console);

    xs_app = this;
    InitXSData();
}

XSApplication::~XSApplication()
{
}
