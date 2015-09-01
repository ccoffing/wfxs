#ifndef XS_BITSTRING_H
#define XS_BITSTRING_H

#include <stdint.h>

/**
 *  An efficient bitstring.
 */
class BitString {
public:
    /**
     *  Creates a bitstring with the specified number of bits.
     *  All bits default to cleared.
     */
    BitString(unsigned int bits);

    BitString(BitString const &);
    BitString &operator=(BitString const &);

    ~BitString();

    unsigned int numBits() const;

    bool test(unsigned int offset) const;

    void clear();
    void clear(unsigned int offset);
    void clear(unsigned int offset, unsigned int n);

    void set();
    void set(unsigned int offset);
    void set(unsigned int offset, unsigned int n);

    void invert();

    void change(unsigned int offset, unsigned int n, bool set);

    unsigned int countSet() const;

    /**
     *  Searches for the first set or cleared bit, from start to stop inclusive.
     *  If no appropriate bit is found, returns past the stop position.
     *  @param[in] start
     *  @param[in] stop
     *  @param[in] set
     *  @return Offset of first set or cleared bit, or > stop if none.
     */
    unsigned int first(unsigned int start, unsigned int stop, bool set);

    unsigned int last(unsigned int start, unsigned int stop, bool set);

private:
    unsigned int m_numBits;
    uint32_t *m_bits;
};

#endif
