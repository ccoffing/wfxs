#include "XSSkeinPalette.h"
#include "Logger.h"
#include "XSDataIO.h"

#define LOG_NAME "xs.SknPal"


XSSkeinPalette::XSSkeinPalette()
    : m_skeins()
{
}

XSSkein const* XSSkeinPalette::lookup(
        Maker_t maker, FlossProductLine_t productLine, std::string const& id)
{
    for (auto& elem : m_skeins) {
        if (elem.m_maker == maker && elem.m_productLine == productLine && elem.m_id == id)
            return &elem;
    }
    return 0;
}

void XSSkeinPalette::serialize(std::ostream& file) const
{
    size_t n = m_skeins.size();

    WriteBE16_exc(file, n);
    for (size_t i = 0; i < n; ++i) {
        const XSSkein& s = m_skeins[i];
        s.serialize(file);
    }
}

void XSSkeinPalette::unserialize(std::istream& src)
{
    uint16_t n;

    ReadBE16_exc(src, n);
    size_t skeins = n;
    Log::debug(LOG_NAME, "Loading %u skeins", n);
    for (size_t i = 0; i < skeins; ++i) {
        XSSkein skein;
        skein.unserialize(src);
        m_skeins.push_back(skein);
    }
}
