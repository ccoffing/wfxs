#ifndef XS_PROPERTIES_H
#define XS_PROPERTIES_H

/** @file
 *  @brief TODO
 */

#include "clc/data/Buffer.h"

#include <iostream>


/**
 */
class XSProperties {
public:
    XSProperties();
    ~XSProperties();
    XSProperties(XSProperties const &ref);
    XSProperties &operator=(XSProperties const &);

    void Serialize(std::ostream &file) const;
    void Unserialize(std::istream &file);

private:
    void SetDefaults();

public:
    clc::Buffer m_filename;  // FIXME:  move this elsewhere
    clc::Buffer m_title;
    clc::Buffer m_author;
    clc::Buffer m_fabric;
    clc::Buffer m_instructions;
    uint32_t m_horiClothCount;
    uint32_t m_vertClothCount;
};

#endif
