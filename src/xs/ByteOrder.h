#ifndef XS_BYTEORDER_H
#define XS_BYTEORDER_H

#include <stdint.h>

/**
 *  Defines architecture dependent endian constants.
 *  The constant reflects the byte order, "4" is the most
 *  significant byte, "1" the least significant one.
 *
 *  Defines:
 *    LITTLE_ENDIAN
 *    BIG_ENDIAN
 *    BYTE_ORDER
 *
 *  Canonical way to check byte order:
 *  \#if BYTE_ORDER == LITTLE_ENDIAN
 *      ....
 *  \#endif
 */
#if defined(__BEOS__) || defined(__HAIKU__)
#include <be/support/SupportDefs.h>
#else

#ifndef __USE_BSD
#define __USE_BSD   // Necessary for Linux
#endif
#include <sys/types.h>

#ifndef BYTE_ORDER
#error  BYTE_ORDER was not defined; check your headers
#endif

#endif

class ByteOrder {
public:
    enum Order {
        LittleEndian,
        BigEndian
    };

    static enum Order nativeOrder()
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        return LittleEndian;
#else
        return BigEndian;
#endif
    }

    static uint16_t swapUInt16(uint16_t x)
    {
        return (uint16_t)((((uint16_t)(x) & 0xff00) >> 8) |
                          (((uint16_t)(x) & 0x00ff) << 8));
    }

    static uint32_t swapUInt32(uint32_t x)
    {
        return (uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) |
                          (((uint32_t)(x) & 0x00ff0000) >>  8) |
                          (((uint32_t)(x) & 0x0000ff00) <<  8) |
                          (((uint32_t)(x) & 0x000000ff) << 24));
    }

    static uint64_t swapUInt64(uint64_t x)
    {
        return (uint64_t)((((uint64_t)(x) & 0xff00000000000000ULL) >> 56) |
                          (((uint64_t)(x) & 0x00ff000000000000ULL) >> 40) |
                          (((uint64_t)(x) & 0x0000ff0000000000ULL) >> 24) |
                          (((uint64_t)(x) & 0x000000ff00000000ULL) >>  8) |
                          (((uint64_t)(x) & 0x00000000ff000000ULL) <<  8) |
                          (((uint64_t)(x) & 0x0000000000ff0000ULL) << 24) |
                          (((uint64_t)(x) & 0x000000000000ff00ULL) << 40) |
                          (((uint64_t)(x) & 0x00000000000000ffULL) << 56));
    }

    static inline uint16_t swapUInt16OnLE(uint16_t x)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        return swapUInt16(x);
#else
        return x;
#endif
    }

    static inline uint32_t swapUInt32OnLE(uint32_t x)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        return swapUInt32(x);
#else
        return x;
#endif
    }

    static inline uint16_t hostToBE16(uint16_t x)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        return swapUInt16(x);
#else
        return x;
#endif
    }

    static inline uint32_t hostToBE32(uint32_t x)
    {
#if BYTE_ORDER == LITTLE_ENDIAN
        return swapUInt32(x);
#else
        return x;
#endif
    }

    /**
     *  Reads a little endian 32 bit integer from a byte stream.
     *  @return 32-bit value as little endian
     */
    static inline uint32_t getUInt32LE(const unsigned char *s)
    {
        return s[0] | s[1] << 8 | s[2] << 16 | s[3] << 24;
    }

    /**
     *  Reads a big endian 32 bit integer from a byte stream.
     *  @return 32-bit value as big endian
     */
    static inline uint32_t getUInt32BE(const char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        return s[3] | (s[2] << 8) | (s[1] << 16) | (s[0] << 24);
    }

    static inline uint32_t unflatten32(const char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        return s[3] | (s[2] << 8) | (s[1] << 16) | (s[0] << 24);
    }

    static inline char *flatten32(uint32_t v, char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        *s++ = (v >> 24);
        *s++ = (v >> 16);
        *s++ = (v >> 8);
        *s++ = v;
        return (char *)s;
    }

    static inline uint16_t unflatten16(const char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        return s[1] | (s[0] << 8);
    }

    static inline char *flatten16(uint16_t v, char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        *s++ = (v >> 8);
        *s++ = v;
        return (char *)s;
    }

    static inline uint8_t unflatten8(const char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        return s[1] | (s[0] << 8);
    }

    static inline char *flatten8(uint8_t v, char *_s)
    {
        unsigned char *s = (unsigned char *)_s;

        *s++ = v;
        return (char *)s;
    }

    static inline uint32_t getPacked(unsigned char msb, unsigned char b, unsigned char c, unsigned char lsb)
    {
        return lsb | c << 8 | b << 16 | msb << 24;
    }
};

#endif
