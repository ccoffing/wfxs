#include "qt/XSPropertiesDialog.h"

#include <QtGui>

PatSizeTab::PatSizeTab(XSProperties &properties) :
    m_properties(properties)
{
}

PatDescrTab::PatDescrTab(XSProperties &properties) :
    m_properties(properties)
{
     QLabel *titleLabel = new QLabel(tr("Title:"));
     QLineEdit *titleEdit = new QLineEdit(m_properties.m_title.c_str());

     QLabel *descrLabel = new QLabel(tr("Description:"));

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(titleLabel);
     mainLayout->addWidget(titleEdit);
     mainLayout->addWidget(descrLabel);
     mainLayout->addStretch(1);
     setLayout(mainLayout);
}

PatInstrTab::PatInstrTab(XSProperties &properties) :
    m_properties(properties)
{
}

XSPropertiesDialog::XSPropertiesDialog(QWidget *parent, XSProperties &properties) :
    QDialog(parent),
    m_properties(properties)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetNoConstraint);

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(new PatSizeTab(m_properties), tr("Pattern Size"));
    tabWidget->addTab(new PatDescrTab(m_properties), tr("Description"));
    tabWidget->addTab(new PatInstrTab(m_properties), tr("Instructions"));
    layout->addWidget(tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonBox);

    setLayout(layout);
    setWindowTitle(tr("Properties"));
}

void XSPropertiesDialog::saveClicked()
{
    // TODO  read from form into m_properties
}
