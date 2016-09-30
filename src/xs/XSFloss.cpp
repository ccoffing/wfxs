#include "XSFloss.h"
#include "Logger.h"
#include "XSData.h"
#include "XSDataIO.h"
#include "XSException.h"
#include "XSSkein.h"
#include "XSSkeinPalette.h"
#include "XSSymbolPalette.h"

#include <cassert>

#define LOG_NAME "xs.Floss"

XSFloss::XSFloss()
    : m_numskeins(0)
    , m_skeins()
    , m_strands()
    , m_symbol(0)
    , m_symbolPalette(0)
{
}

XSFloss::~XSFloss()
{
}

XSFloss::XSFloss(std::istream& src, XSSymbolPalette* symbolPalette)
    : m_numskeins(0)
    , m_skeins()
    , m_strands()
    , m_symbol(0)
    , m_symbolPalette(symbolPalette)
{
    unserialize(src);
}

void XSFloss::serialize(std::ostream& file) const
{
    Write8_exc(file, m_numskeins);
    for (unsigned int skein = 0; skein < m_numskeins; ++skein) {
        Write8_exc(file, m_strands[skein]);
        m_skeins[skein]->serializeRef(file);
    }
    WriteCStr_exc(file, m_symbol);
}

void XSFloss::unserialize(std::istream& src)
{
    uint8_t numskeins;

    Read8_exc(src, numskeins);
    if (!numskeins || numskeins > maxSkeins)
        throw IllegalFormatException();
    m_numskeins = numskeins;
    for (unsigned int i = 0; i < m_numskeins; ++i) {
        uint8_t strands;
        Read8_exc(src, strands);
        if (strands > 6 || strands == 0)
            throw IllegalFormatException();
        m_strands[i] = strands;

        Maker_t maker;
        FlossProductLine_t productLine;
        std::string id;
        XSSkein::unserializeRef(src, maker, productLine, id);
        Log::info(LOG_NAME, "Floss:  skein ref:  maker %d   prodLine %d   id %s", maker,
                productLine, id.c_str());

        XSSkein const* skein = makerSkeinPalette[maker]->lookup(maker, productLine, id);
        if (skein) {
            m_skeins[i] = skein;
        } else {
            // The appropriate maker, product line, or ID wasn't read in
            // previously.
            throw IllegalFormatException();
        }
    }

    uint32_t symbol;
    char* symbolp = (char*)&symbol;
    ReadCStr_exc(src, symbolp, sizeof(symbol));
    m_symbolPalette->reserveSymbol(m_symbolPalette->addSymbol(symbolp));
    setSymbol(symbolp);
}

XSFloss::XSFloss(XSSkein const* skein, unsigned int strands, XSSymbolPalette* symbolPalette)
    : m_numskeins(1)
    , m_skeins()
    , m_strands()
    , m_symbol(0)
    , m_symbolPalette(symbolPalette)
{
    m_skeins[0] = skein;
    m_strands[0] = strands;

    setSymbol(m_symbolPalette->reserveNextSymbol());  // FIXME:  no more?
}

#if 0
XSFloss::XSFloss(XSSkein const *skein0,
        unsigned int strands0,
        XSSkein const *skein1,
        unsigned int strands1) :
    m_numskeins(2),
    m_skeins(),
    m_strands()
{
    m_skeins[0] = skein0;
    m_strands[0] = strands0;
    m_skeins[1] = skein1;
    m_strands[1] = strands1;
    setSymbol("#");  // FIXME
}

#endif

bool XSFloss::addSkein(XSSkein const* skein, unsigned int strands)
{
    if (m_numskeins == 0) {
        m_skeins[0] = skein;
        m_strands[0] = strands;
        m_numskeins++;

        assert(m_numskeins <= maxSkeins);
    } else {
        if (m_skeins[0] == skein) {
            m_strands[0] += strands;
        } else if (m_numskeins == 1) {
            m_skeins[1] = skein;
            m_strands[1] = strands;
            m_numskeins++;
        } else {
            assert(m_numskeins == 2);
            assert(m_skeins[1] == skein);
            m_strands[1] += strands;
        }
    }
    return true;
}

XSColor XSFloss::getColor() const
{
    unsigned int red = 0, green = 0, blue = 0;
    unsigned int mult = 0;

    for (unsigned int skein = 0; skein < m_numskeins; ++skein) {
        mult++;
        for (unsigned int strand = 0; strand < m_strands[skein]; ++strand) {
            // FIXME
            red += m_skeins[skein]->m_colors[0].red;
            green += m_skeins[skein]->m_colors[0].green;
            blue += m_skeins[skein]->m_colors[0].blue;
        }
    }
    if (mult == 0) {
        XSColor c(0, 0, 0);
        return c;
    }
    XSColor c(red / mult, green / mult, blue / mult);
    return c;
}

void XSFloss::setSymbol(char const* symbol)
{
    // FIXME:  copy only the first UTF8 character
    m_symbol = symbol;
}
