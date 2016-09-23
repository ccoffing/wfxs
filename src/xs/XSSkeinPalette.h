#ifndef XS_SKEIN_PALETTE
#define XS_SKEIN_PALETTE

/** @file
 *  A collection of floss skeins.
 */

#include <vector>

#include "XSSkein.h"

/**
 *  A collection of floss skeins.
 */
class XSSkeinPalette {
public:
    XSSkeinPalette();

    XSSkein const* Lookup(Maker_t maker, FlossProductLine_t productLine, std::string const& id);

    void Serialize(std::ostream& file) const;
    void Unserialize(std::istream& src);

private:
    std::vector<XSSkein> m_skeins;
};

#endif
