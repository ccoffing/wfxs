#ifndef XS_STRING_UTILS_H
#define XS_STRING_UTILS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

std::string formatList(const char *fmt, va_list argList)
{
    std::string s;

#if 1 // TODO:  HAVE_VASPRINTF
    char *buf;
    int len = vasprintf(&buf, fmt, argList);
    if (len >= 0) {
        s.assign(buf, len);
        free(buf);
    }
#else
    va_list argList2;
    va_copy(argList2, argList);
    int len = vsnprintf(NULL, 0, fmt, argList2) + 1;  // measure,
    va_end(argList2);
    char *buf = (char *)alloca(len);
    int printed = vsnprintf(buf, len, fmt, argList); // format,
    assert(printed + 1 == len);
    (void)printed;
    _DetachWith(buf, len - 1, len - 1);              // copy
#endif
    return s;
}

std::string format(const char *fmt, ...)
{
    va_list argList;

    va_start(argList, fmt);
    std::string s = formatList(fmt, argList);
    va_end(argList);
    return s;
}

std::string &appendFormatList(std::string &s, const char *fmt, va_list argList)
{
#if 1  // TODO:  HAVE_VASPRINTF
    char *buf;
    int len = vasprintf(&buf, fmt, argList);
    if (len >= 0) {
        s.append(buf, len);
        free(buf);
    }
#else
    va_list argList2;
    va_copy(argList2, argList);
    int len = vsnprintf(NULL, 0, fmt, argList2) + 1;  // measure,
    va_end(argList2);
    char *buf = (char *)alloca(len);
    int printed = vsnprintf(buf, len, fmt, argList); // format,
    assert(printed + 1 == len);
    (void)printed;
    _DoAppend(buf, len - 1);                         // copy
#endif
    return s;
}

std::string &appendFormat(std::string &s, const char *fmt, ...)
{
    va_list argList;

    va_start(argList, fmt);
    appendFormatList(s, fmt, argList);
    va_end(argList);
    return s;
}


#endif
