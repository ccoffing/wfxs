#include "LogAppenders.h"
#include "Logger.h"
#include "XSApplication.h"
#include "XSData.h"


XSApplication *xs_app;
static LogAppenderCFile console(stderr);

XSApplication::XSApplication(int argc, char *argv[])
{
    int i = 0;
    int verbose = 0;

    while (++i < argc) {
        if (strcmp(argv[i], "-v") == 0)
            verbose++;
    }

    Log::Level level;
    switch (verbose) {
    case 0:
        level = Log::Warn;
        break;
    case 1:
        level = Log::Info;
        break;
    case 2:
        level = Log::Debug;
        break;
    default:
        level = Log::Trace;
        break;
    }
    Log::get("")->setLevel(level);
    Log::get("")->setAppender(&console);

    xs_app = this;
    InitXSData();
}

XSApplication::~XSApplication()
{
}
