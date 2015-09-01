#ifndef XS_FLOSS_PALETTE_H
#define XS_FLOSS_PALETTE_H

#include "xs/XSSymbolPalette.h"

#include <iostream>


class XSFloss;

/**
 */
class XSFlossPalette {
public:
    XSFlossPalette();
    XSFlossPalette(XSFlossPalette const &);
    XSFlossPalette(std::istream &src);
    ~XSFlossPalette();

    XSFlossPalette &operator=(XSFlossPalette const &);
    XSFloss &operator[](unsigned int i) const;
    XSFloss &operator[](unsigned int i);

    XSFloss &GetFloss(unsigned int i) const;
    unsigned int size() const;

    void Serialize(std::ostream &file) const;
    void Unserialize(std::istream &src);
    void UnserializeAll(std::istream &src);

private:
    void ResizePalette(unsigned int);

    unsigned int m_paletteSize;
    XSFloss *m_palette;
    XSSymbolPalette m_symbolPalette;
};

#endif
