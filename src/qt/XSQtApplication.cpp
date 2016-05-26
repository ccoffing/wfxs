#include "qt/XSQtApplication.h"
#include "xs/Logger.h"

XSQtApplication::XSQtApplication(int &argc, char *argv[]) :
    QApplication(argc, argv),
    m_app(argc, argv)
{
    m_window.show();
}
