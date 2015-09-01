#ifndef XS_FILE_H
#define XS_FILE_H

// FIXME:
// This is only for the ProgressFn.  Seems that should go in some ABC for all the file loaders
// that FileProber uses....
typedef void (*ProgressFn)(void *handle, unsigned int percent);

#include "XSController.h"

class XSFileIn {
public:
    XSFileIn(std::istream &file, ProgressFn *progress, void *handle);
    virtual ~XSFileIn();

    static bool IsHandled(std::istream &file);

    virtual XSController *Read() = 0;

protected:
    std::istream &m_file;
    ProgressFn *m_progressFn;
    void *m_handle;

private:
    // Disallow copying
    XSFileIn(const XSFileIn &);
    XSFileIn &operator=(const XSFileIn &);
};

class XSFileOut {
public:
    XSFileOut(std::ostream &file, ProgressFn *progress, void *handle);
    virtual ~XSFileOut();

    virtual void Write(XSController *) = 0;

protected:
    std::ostream &m_file;
    ProgressFn *m_progressFn;
    void *m_handle;

private:
    // Disallow copying
    XSFileOut(const XSFileOut &);
    XSFileOut &operator=(const XSFileOut &);
};

#endif
