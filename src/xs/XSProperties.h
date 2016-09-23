#ifndef XS_PROPERTIES_H
#define XS_PROPERTIES_H

/** @file
 *  @brief TODO
 */

#include <iostream>

/**
 */
class XSProperties {
public:
    XSProperties();
    ~XSProperties();
    XSProperties(XSProperties const& ref);
    XSProperties& operator=(XSProperties const&);

    void Serialize(std::ostream& file) const;
    void Unserialize(std::istream& file);

private:
    void SetDefaults();

public:
    std::string m_filename;  // FIXME:  move this elsewhere
    std::string m_title;
    std::string m_author;
    std::string m_fabric;
    std::string m_instructions;
    uint32_t m_horiClothCount;
    uint32_t m_vertClothCount;
};

#endif
