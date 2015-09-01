#ifndef XS_DOCUMENT_H
#define XS_DOCUMENT_H

#include "XSModel.h"


/**
 *  Aggregation of filename, is modified status, model, etc
 */
class XSDocument {
public:
    XSDocument();

protected:
    std::string m_filename;
    bool m_isModified;
    XSModel m_model;
};

#endif
