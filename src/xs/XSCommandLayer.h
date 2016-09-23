#ifndef XS_COMMAND_LAYER_H
#define XS_COMMAND_LAYER_H

#include "XSCommand.h"


/**
 */
class XSCommandLayerAdd : public XSCommand {
public:
    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;
};


/**
 */
class XSCommandLayerDel : public XSCommand {
public:
    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;
};


/**
 */
class XSCommandLayerUp : public XSCommand {
public:
    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;
};


/**
 */
class XSCommandLayerDown : public XSCommand {
public:
    virtual int Do();
    virtual int Undo();
    virtual char const* GetDescription() const;
};


#endif
