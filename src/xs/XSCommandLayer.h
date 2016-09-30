#ifndef XS_COMMAND_LAYER_H
#define XS_COMMAND_LAYER_H

#include "XSCommand.h"


/**
 */
class XSCommandLayerAdd : public XSCommand {
public:
    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;
};


/**
 */
class XSCommandLayerDel : public XSCommand {
public:
    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;
};


/**
 */
class XSCommandLayerUp : public XSCommand {
public:
    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;
};


/**
 */
class XSCommandLayerDown : public XSCommand {
public:
    virtual int doCommand();
    virtual int undoCommand();
    virtual char const* getDescription() const;
};


#endif
