#include "xs/BitOps.h"
#include "xs/BitString.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

inline unsigned int getWords(unsigned int bits)
{
    return (bits + 31) >> 5;
}

inline unsigned int getFullWords(unsigned int bits)
{
    return bits >> 5;
}

inline unsigned int getFinalMask(unsigned int bits)
{
    return (1 << (bits & 0x0000001f)) - 1;
}


BitString::BitString(unsigned int bits) :
    m_numBits(bits),
    m_bits(new uint32_t[getWords(m_numBits)])
{
    clear();
}


BitString::BitString(BitString const &rhs) :
    m_numBits(rhs.m_numBits),
    m_bits(new uint32_t[getWords(m_numBits)])
{
    memcpy(m_bits, rhs.m_bits, (m_numBits + 7) >> 3);
}


BitString &BitString::operator=(BitString const &rhs)
{
    if (this != &rhs) {
        m_numBits = rhs.m_numBits;
        delete[] m_bits;
        m_bits = new uint32_t[getWords(m_numBits)];
        memcpy(m_bits, rhs.m_bits, (m_numBits + 7) >> 3);
    }
    return *this;
}

BitString::~BitString()
{
    delete[] m_bits;
}


bool BitString::test(unsigned int i) const
{
    assert(i < m_numBits);
    uint32_t *set = m_bits;
    set += (i >> 5);
    return *set & (1 << (i & 31));
}


void BitString::clear()
{
    for (unsigned int i = 0; i < getWords(m_numBits); ++i) {
        m_bits[i] = 0;
    }
}


void BitString::invert()
{
    for (unsigned int i = 0; i < getWords(m_numBits); ++i) {
        m_bits[i] = ~m_bits[i];
    }
}


void BitString::clear(unsigned int i)
{
    assert(i < m_numBits);
    uint32_t *set = m_bits;
    set += (i >> 5);
    *set &= ~(1 << (i & 31));
}


void BitString::clear(unsigned int offset,
        unsigned int n)
{
    assert(offset < m_numBits);
    assert(n <= m_numBits);
    assert(offset <= m_numBits - offset);
    // OPTIMIZE
    for (unsigned int i = offset; i < offset + n; ++i) {
        clear(i);
    }
}


void BitString::set()
{
    for (unsigned int i = 0; i < getWords(m_numBits); ++i) {
        m_bits[i] = (uint32_t)-1;
    }
}


void BitString::set(unsigned int i)
{
    assert(i < m_numBits);
    uint32_t *set = m_bits;
    set += (i >> 5);
    *set |= 1 << (i & 31);
}


void BitString::set(unsigned int offset,
        unsigned int n)
{
    assert(offset < m_numBits);
    assert(n <= m_numBits);
    assert(offset <= m_numBits - offset);
    // OPTIMIZE
    for (unsigned int i = offset; i < offset + n; ++i) {
        set(i);
    }
}


void BitString::change(unsigned int i,
        unsigned int n,
        bool doSet)
{
    if (doSet)
        set(i, n);
    else
        clear(i, n);
}


unsigned int BitString::first(unsigned int start,
        unsigned int stop,
        bool set)
{
    unsigned int i;

    for (i = start; i <= stop; ++i) {
        // OPTIMIZATION:  use ntz
        if (test(i) == set)
            break;
    }
    return i;
}


unsigned int BitString::countSet() const
{
    unsigned int total = 0;
    unsigned int fullWords = getFullWords(m_numBits);
    unsigned int words = getWords(m_numBits);

    for (unsigned int word = 0; word < fullWords; ++word) {
        uint32_t i = m_bits[word];
        total += population(i);
    }
    if (words > fullWords)
        total += population(m_bits[fullWords] & getFinalMask(m_numBits));
    return total;
}


unsigned int BitString::numBits() const
{
    return m_numBits;
}
