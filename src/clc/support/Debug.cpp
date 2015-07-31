#include "clc/data/Buffer.h"
#include "clc/support/Debug.h"

#include <stdio.h>
#include <stdlib.h>
#if defined(__linux__)
#include <sys/prctl.h>
#endif


namespace clc {

int Debugger::enter()
{
    volatile int r = 0;

#ifdef HAVE_BUILTIN_TRAP
    __builtin_trap();
#else
    *((volatile int *)0) = 1;
#endif
    return r;
}

void Debugger::print(char const *str)
{
    printf("%s", str);
}

void Debugger::printf(const Buffer &b)
{
    Debugger::print(b.c_str());
}

void Debugger::printf(char const *fmt, va_list ap)
{
    vprintf(fmt, ap);  // TODO:  various destinations
}

void Debugger::printf(char const *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Debugger::printf(fmt, ap);
    va_end(ap);
}

int Debugger::asserted(char const *file, int line, char const *expr)
{
    Debugger::printf("%s:%u: failed assertion `%s'\n", file, line, expr);
    int r = Debugger::enter();

    if (r == 0)
        abort();
    return r;
}

void Debugger::nameThread(const char *name)
{
#if DEBUG
#if defined(__linux__)
    prctl(PR_SET_NAME, (unsigned long)name);
#else
    (void)name;
#endif
#else
    (void)name;
#endif
}

}
