#include "qt/XSPropertiesDialog.h"

#include <QtGui>


XSPropertiesDialog::XSPropertiesDialog(QWidget *parent, XSProperties &properties) :
    QDialog(parent),
    m_properties(properties)
{
    QVBoxLayout *layout = new QVBoxLayout;

    // TODO
    // Pattern Size
    // Description
    // Instructions

    QPushButton *saveButton = new QPushButton(tr("&Save"));

    saveButton->setDefault(true);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);
    setLayout(layout);
}

void XSPropertiesDialog::saveClicked()
{
    // TODO  read from form into m_properties

    close();
}
