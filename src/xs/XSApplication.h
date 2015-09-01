#ifndef XS_APPLICATION_H
#define XS_APPLICATION_H


/** Cross stitch application
 */
class XSApplication {
public:
    XSApplication(int argc, char *argv[]);
    virtual ~XSApplication();

private:
    // Disallow copying
    XSApplication(XSApplication const &);
    XSApplication &operator=(XSApplication const &);
};

extern XSApplication *xs_app;

#endif
