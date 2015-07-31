#ifndef XS_FLOSS_H
#define XS_FLOSS_H

/** @file
 *  Floss
 */

#include <iostream>

class XSSkein;
class XSSymbolPalette;
class XSColor;


/**
 *  Floss, which associates metadata (such as symbol and name) with (possibly) several strands of
 *  (possibly) several skeins.
 */
class XSFloss
{
public:
    static const unsigned int MaxSkeins = 2;

    XSFloss();
    ~XSFloss();
    XSFloss(std::istream &src, XSSymbolPalette *);
    XSFloss(XSSkein const *skein, unsigned int number, XSSymbolPalette *);

    bool AddSkein(XSSkein const *skein, unsigned int number);

    XSColor GetColor() const;

    void SetSymbol(char const *);

    void Serialize(std::ostream &file) const;
    void Unserialize(std::istream &src);
    unsigned int m_numskeins;
    XSSkein const *m_skeins[MaxSkeins];

    unsigned int m_strands[MaxSkeins];

    /**
     *  The m_symbol pointer points to an entry in the m_symbolPalette.
     *  It is not a copy.
     */
    char const *m_symbol;
    XSSymbolPalette *m_symbolPalette;
};

#endif
