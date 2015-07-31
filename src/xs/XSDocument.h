#ifndef XS_DOCUMENT_H
#define XS_DOCUMENT_H

#include "clc/data/Buffer.h"

#include "XSModel.h"


/**
 *  Aggregation of filename, is modified status, model, etc
 */
class XSDocument {
public:
    XSDocument();

protected:
    clc::Buffer m_filename;
    bool m_isModified;
    XSModel m_model;
};

#endif
