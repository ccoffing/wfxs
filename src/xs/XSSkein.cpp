#include "Logger.h"
#include "XSDataIO.h"
#include "XSException.h"
#include "XSSkein.h"
#include "XSSymbolPalette.h"

#define LOG_NAME "xs.Skein"


XSSkein::XSSkein() :
    m_maker((Maker_t)-1),
    m_productLine((FlossProductLine_t)-1),
    m_id(),
    m_description(),
    m_colors(),
    m_discontinued(false),
    m_blend(FB_NONE)
{
}

XSSkein::~XSSkein()
{
}

XSSkein::XSSkein(Maker_t maker,
        FlossProductLine_t productLine,
        const char *id,
        const char *description,
        XSColor color,
        bool discontinued,
        FlossBlend_t blend) :
    m_maker(maker),
    m_productLine(productLine),
    m_id(id),
    m_description(description),
    m_colors(),
    m_discontinued(discontinued),
    m_blend(blend)
{
    m_colors.push_back(color);
}

XSSkein::XSSkein(std::istream &src)
{
    Unserialize(src);
}

void XSSkein::Unserialize(std::istream &src)
{
    uint8_t ui8;

    Read8_exc(src, ui8);
    if (ui8 >= FM_NUMBER)
        throw IllegalFormatException();
    m_maker = (Maker_t)ui8;

    Read8_exc(src, ui8);
    if (ui8 >= FPL_NUMBER)
        throw IllegalFormatException();
    m_productLine = (FlossProductLine_t)ui8;

    ReadCStr_exc(src, m_id, MAX_FLOSS_ID_LEN);

    ReadCStr_exc(src, m_description, MAX_FLOSS_DESC_LEN);

    // flags
    Read8_exc(src, ui8);
    m_discontinued = (ui8 & 1);
    unsigned int blend = (ui8 >> 1) & 3;
    if (blend >= FB_NUMBER)
        throw IllegalFormatException();
    m_blend = (FlossBlend_t)blend;

    Read8_exc(src, ui8);
    unsigned int color = 0;
    unsigned int colors = ui8;
    for (unsigned int i = 0; i < colors; ++i) {
        uint8_t r, g, b;
        Read8_exc(src, r);
        Read8_exc(src, g);
        Read8_exc(src, b);
        XSColor rgb(r, g, b);
        m_colors.push_back(rgb);
        color += ((r << 16) | (g << 8) | b);
    }
    Log::trace(LOG_NAME, "%d %d %d", m_maker, m_productLine, colors);
}

void XSSkein::Serialize(std::ostream &file) const
{
    Write8_exc(file, (uint8_t)m_maker);
    Write8_exc(file, (uint8_t)m_productLine);
    WriteCStr_exc(file, m_id);
    WriteCStr_exc(file, m_description);

    uint8_t flags;
    flags = (m_discontinued ? 1 : 0);
    flags |= (m_blend << 1);
    Write8_exc(file, flags);

    Write8_exc(file, m_colors.size());
    // printf("Skein writing out %zd colors\n", m_colors.size());
    for (unsigned int i = 0; i < m_colors.size(); ++i) {
        XSColor rgb = m_colors[i];
        Write8_exc(file, rgb.red);
        Write8_exc(file, rgb.green);
        Write8_exc(file, rgb.blue);
    }
}

void XSSkein::UnserializeRef(std::istream &src,
        Maker_t &maker,
        FlossProductLine_t &productLine,
        std::string &id)
{
    uint8_t ui8;

    Read8_exc(src, ui8);
    if (ui8 > FM_NUMBER)
        throw IllegalFormatException();
    maker = (Maker_t)ui8;

    Read8_exc(src, ui8);
    if (ui8 > FPL_NUMBER)
        throw IllegalFormatException();
    productLine = (FlossProductLine_t)ui8;

    ReadCStr_exc(src, id, MAX_FLOSS_ID_LEN);
}

void XSSkein::SerializeRef(std::ostream &file) const
{
    Write8_exc(file, m_maker);
    Write8_exc(file, m_productLine);
    WriteCStr_exc(file, m_id);
}

XSSkein::XSSkein(XSSkein const &rhs) :
    m_maker(rhs.m_maker),
    m_productLine(rhs.m_productLine),
    m_id(rhs.m_id),
    m_description(rhs.m_description),
    m_colors(rhs.m_colors),
    m_discontinued(rhs.m_discontinued),
    m_blend(rhs.m_blend)
{
}

XSSkein &XSSkein::operator=(XSSkein const &rhs)
{
    if (this != &rhs) {
        m_maker        = rhs.m_maker;
        m_productLine  = rhs.m_productLine;
        m_id           = rhs.m_id;

        m_description  = rhs.m_description;
        m_colors       = rhs.m_colors;
        m_discontinued = rhs.m_discontinued;
        m_blend        = rhs.m_blend;
    }
    return *this;
}

#if 0
bool XSSkein::operator==(XSSkein const &rhs) const
{
    return m_maker == rhs.m_maker &&
           m_productLine == rhs.m_productLine &&
           m_id == rhs.m_id;
}

#endif
