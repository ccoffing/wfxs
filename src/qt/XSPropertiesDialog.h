#ifndef QT_XS_PROPERTIES_DIALOG_H
#define QT_XS_PROPERTIES_DIALOG_H

#include "xs/XSModel.h"

#include <QDialog>


class XSPropertiesDialog : public QDialog {
    Q_OBJECT

public:
    XSPropertiesDialog(QWidget *parent, XSProperties &properties);

protected:
    XSProperties &m_properties;

private slots:
    void saveClicked();

private:
};

#endif
