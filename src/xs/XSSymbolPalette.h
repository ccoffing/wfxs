#ifndef XS_SYMBOL_PALETTE_H
#define XS_SYMBOL_PALETTE_H

/**
 */
class XSSymbolPalette {
public:
    XSSymbolPalette();
    XSSymbolPalette(XSSymbolPalette const& rhs);
    ~XSSymbolPalette();

    char const* reserveNextSymbol();
    char const* reserveSymbol(unsigned int index);
    void freeSymbol(char const*);
    unsigned int addSymbol(char const*);

    char const* symbolAtIndex(unsigned int i);
    char* editSymbolAtIndex(unsigned int i);

    unsigned int numStatic;
    unsigned int numTotal;
    bool* isUsed;
    char* symbols;

private:
    int m_lastReservedIndex;
};

#endif
