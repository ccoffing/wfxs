#ifndef XS_SYMBOL_PALETTE_H
#define XS_SYMBOL_PALETTE_H

/**
 */
class XSSymbolPalette {
public:
    XSSymbolPalette();
    XSSymbolPalette(XSSymbolPalette const &rhs);
    ~XSSymbolPalette();

    char const *ReserveNextSymbol();
    char const *ReserveSymbol(unsigned int index);
    void FreeSymbol(char const *);
    unsigned int AddSymbol(char const *);

    char const *SymbolAtIndex(unsigned int i);
    char *EditSymbolAtIndex(unsigned int i);

    unsigned int numStatic;
    unsigned int numTotal;
    bool *isUsed;
    char *symbols;

private:
    int m_lastReservedIndex;
};

#endif
