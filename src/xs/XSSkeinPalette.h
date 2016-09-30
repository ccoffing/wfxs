#ifndef XS_SKEIN_PALETTE
#define XS_SKEIN_PALETTE

/** @file
 *  A collection of floss skeins.
 */

#include "XSSkein.h"

#include <vector>

/**
 *  A collection of floss skeins.
 */
class XSSkeinPalette {
public:
    XSSkeinPalette();

    XSSkein const* lookup(Maker_t maker, FlossProductLine_t productLine, std::string const& id);

    void serialize(std::ostream& file) const;
    void unserialize(std::istream& src);

private:
    std::vector<XSSkein> m_skeins;
};

#endif
