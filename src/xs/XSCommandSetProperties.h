#ifndef XS_COMMAND_SET_PROPERTIES_H
#define XS_COMMAND_SET_PROPERTIES_H

#include "XSCommand.h"
#include "XSProperties.h"

/**
 */
class XSCommandSetProperties : public XSCommand {
public:
    XSCommandSetProperties(XSProperties const &properties);

    virtual int Do();
    virtual int Undo();
    virtual char const *GetDescription() const;

private:
    XSProperties m_oldProperties;
    XSProperties m_newProperties;

    // Disallow copying
    XSCommandSetProperties(XSCommandSetProperties const &);
    XSCommandSetProperties &operator=(XSCommandSetProperties const &);
};

#endif
