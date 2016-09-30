#ifndef XS_FLOSS_PALETTE_H
#define XS_FLOSS_PALETTE_H

#include "XSSymbolPalette.h"

#include <iostream>


class XSFloss;

/**
 */
class XSFlossPalette {
public:
    XSFlossPalette();
    XSFlossPalette(XSFlossPalette const&);
    XSFlossPalette(std::istream& src);
    ~XSFlossPalette();

    XSFlossPalette& operator=(XSFlossPalette const&);
    XSFloss& operator[](unsigned int i) const;
    XSFloss& operator[](unsigned int i);

    XSFloss& getFloss(unsigned int i) const;
    unsigned int size() const;

    void serialize(std::ostream& file) const;
    void unserialize(std::istream& src);
    void unserializeAll(std::istream& src);

private:
    void resizePalette(unsigned int);

    unsigned int m_paletteSize;
    XSFloss* m_palette;
    XSSymbolPalette m_symbolPalette;
};

#endif
