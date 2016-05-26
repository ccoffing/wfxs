#ifndef XS_QT_APPLICATION_H
#define XS_QT_APPLICATION_H

#include "xs/XSApplication.h"
#include "qt/XSQtEditWindow.h"

#include <QApplication>

class XSQtApplication : public QApplication {
    Q_OBJECT

public:
    XSQtApplication(int &argc, char *argv[]);

protected:
    XSApplication m_app;
    XSQtEditWindow m_window;
};

#endif
