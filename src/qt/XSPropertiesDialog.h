#ifndef QT_XS_PROPERTIES_DIALOG_H
#define QT_XS_PROPERTIES_DIALOG_H

#include "xs/XSModel.h"

#include <QDialog>


class PatSizeTab : public QWidget {
    Q_OBJECT

public:
    PatSizeTab(XSProperties &properties);

protected:
    XSProperties &m_properties;
};

class PatDescrTab : public QWidget {
    Q_OBJECT

public:
    PatDescrTab(XSProperties &properties);

protected:
    XSProperties &m_properties;
};

class PatInstrTab : public QWidget {
    Q_OBJECT

public:
    PatInstrTab(XSProperties &properties);

protected:
    XSProperties &m_properties;
};

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
