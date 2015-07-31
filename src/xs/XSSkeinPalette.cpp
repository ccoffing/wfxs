#include "XSDataIO.h"
#include "XSSkeinPalette.h"

#include "clc/support/ByteOrder.h"
#include "clc/support/Logger.h"

#define LOG_NAME "xs.SknPal"


XSSkeinPalette::XSSkeinPalette() :
    m_skeins()
{
}

XSSkein const *XSSkeinPalette::Lookup(Maker_t maker,
        FlossProductLine_t productLine,
        clc::Buffer const &id)
{
    for (size_t i = 0; i < m_skeins.size(); ++i) {
        if (m_skeins[i].m_maker == maker &&
            m_skeins[i].m_productLine == productLine &&
            m_skeins[i].m_id == id)
            return &m_skeins[i];
    }
    return 0;
}

void XSSkeinPalette::Serialize(std::ostream &file) const
{
    size_t n = m_skeins.size();

    WriteBE16_exc(file, n);
    for (size_t i = 0; i < n; ++i) {
        const XSSkein &s = m_skeins[i];
        s.Serialize(file);
    }
}

void XSSkeinPalette::Unserialize(std::istream &src)
{
    uint16_t n;

    ReadBE16_exc(src, n);
    size_t skeins = n;
    clc::Log::debug(LOG_NAME, "Loading %u skeins", n);
    for (size_t i = 0; i < skeins; ++i) {
        XSSkein skein;
        skein.Unserialize(src);
        m_skeins.push_back(skein);
    }
}
