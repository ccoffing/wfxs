#ifndef XS_FILE_PC_STITCH_H
#define XS_FILE_PC_STITCH_H

#include "XSFile.h"


/**
 */
class XSFileInPCStitch : public XSFileIn {
public:
    XSFileInPCStitch(std::istream &file,
            ProgressFn *progressFn,
            void *handle);
    ~XSFileInPCStitch();

    static bool IsHandled(std::istream &file);

    virtual XSController *Read();
};

/**
 */
class XSFileOutPCStitch : public XSFileOut {
public:
    XSFileOutPCStitch(std::ostream &file,
            ProgressFn *progressFn,
            void *handle);
    ~XSFileOutPCStitch();

    virtual void Write(XSController *);
};

#endif
