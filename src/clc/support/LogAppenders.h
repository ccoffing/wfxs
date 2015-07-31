#ifndef LIBCLC_LOG_APPENDERS_H
#define LIBCLC_LOG_APPENDERS_H

#include "clc/support/Debug.h"
#include "clc/support/Logger.h"

#include <stdio.h>


namespace clc {

class Logger;


/**
 *  Appender that discards messages.
 */
class LogAppenderNull : public LogAppender {
public:
    void append(Buffer &)
    {
    }
};


/**
 *  Appender that sends the messages to the debugger (if any).
 */
class LogAppenderDebugger : public LogAppender {
public:
    void append(Buffer &s)
    {
        Debugger::printf(s);
    }
};


/**
 *  Appender that appends to C-style FILE objects, such as stdout.
 */
class LogAppenderCFile : public LogAppender {
public:
    LogAppenderCFile(FILE *f) :
        m_f(f)
    {
    }

    void append(Buffer &s)
    {
        fprintf(m_f, "%s", s.c_str());
    }

protected:
    FILE *m_f;
};
}

#endif
