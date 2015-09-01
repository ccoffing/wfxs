#include "Logger.h"
#include "XSDataIO.h"
#include "XSFloss.h"
#include "XSFlossPalette.h"

#include <assert.h>

#define LOG "wfxs.FlossPalette"


XSFlossPalette::XSFlossPalette() :
    m_paletteSize(0),
    m_palette(0),
    m_symbolPalette()
{
}

XSFlossPalette::XSFlossPalette(XSFlossPalette const &flossPalette) :
    m_paletteSize(0),
    m_palette(0),
    m_symbolPalette(flossPalette.m_symbolPalette)
{
    ResizePalette(flossPalette.m_paletteSize);
    for (unsigned int i = 0; i < m_paletteSize; ++i) {
        m_palette[i] = flossPalette.m_palette[i];
    }
}

XSFlossPalette::XSFlossPalette(std::istream &src) :
    m_paletteSize(0),
    m_palette(0),
    m_symbolPalette()
{
    UnserializeAll(src);
}

void XSFlossPalette::UnserializeAll(std::istream &src)
{
    unsigned int i = 0;

    while (!src.eof()) {
        XSFloss floss(src, &m_symbolPalette);
        // FIXME:  resizing each time is inefficient
        (*this)[i++] = floss;
    }
}

void XSFlossPalette::Unserialize(std::istream &src)
{
    unsigned int i = 0;
    uint16_t size;

    ReadBE16_exc(src, size);
    while (size--) {
        XSFloss floss(src, &m_symbolPalette);
        // FIXME:  resizing each time is inefficient
        (*this)[i++] = floss;
    }
}

void XSFlossPalette::Serialize(std::ostream &file) const
{
    WriteLE32_exc(file, m_paletteSize);
    for (unsigned int i = 0; i < m_paletteSize; ++i) {
        m_palette[i].Serialize(file);
    }
}

XSFlossPalette &XSFlossPalette::operator=(XSFlossPalette const &rhs)
{
    ResizePalette(rhs.m_paletteSize);
    for (unsigned int i = 0; i < m_paletteSize; ++i) {
        m_palette[i] = rhs.m_palette[i];
    }
    return *this;
}

XSFlossPalette::~XSFlossPalette()
{
    delete[] m_palette;
}

void XSFlossPalette::ResizePalette(unsigned int newSize)
{
    XSFloss *f = new XSFloss[newSize];

    if (m_palette) {
        unsigned int toCopy = (newSize < m_paletteSize) ? newSize : m_paletteSize;
        for (unsigned int i = 0; i < toCopy; ++i) {
            f[i] = m_palette[i];
        }
        delete[] m_palette;
    }
    m_palette = f;
    m_paletteSize = newSize;
}

XSFloss &XSFlossPalette::operator[](unsigned int i)
{
    assert(!(i > m_paletteSize));
    if (i == m_paletteSize)
        ResizePalette(m_paletteSize + 1);
    return m_palette[i];
}

XSFloss &XSFlossPalette::operator[](unsigned int i) const
{
    assert(!(i >= m_paletteSize));
    return m_palette[i];
}

XSFloss &XSFlossPalette::GetFloss(unsigned int i) const
{
    assert(i < m_paletteSize);
    return m_palette[i];
}

unsigned int XSFlossPalette::size() const
{
    return m_paletteSize;
}
