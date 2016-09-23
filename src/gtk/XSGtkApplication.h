#ifndef XS_GTK_APPLICATION_H
#define XS_GTK_APPLICATION_H

#include "xs/XSApplication.h"

class XSGtkApplication : public XSApplication {
public:
    XSGtkApplication(int& argc, char* argv[]);
};

#endif
