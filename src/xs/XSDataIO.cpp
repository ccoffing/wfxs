#include "XSDataIO.h"

#include "clc/data/Buffer.h"
#include "clc/support/Exception.h"

#include <assert.h>
#include <iostream>
#include <string.h>


size_t Write_exc(std::ostream &dio,
        char const *src,
        size_t size)
{
    if (dio.write(src, size).fail())
        throw clc::IOException();
    return size;
}

size_t Write8_exc(std::ostream &dio,
        uint8_t i)
{
    return Write_exc(dio, (char const *)&i, 1);
}

size_t WriteLE16_exc(std::ostream &dio,
        uint16_t i)
{
    i = clc::ByteOrder::swapUInt16OnLE(i);
    return Write_exc(dio, (char const *)&i, 2);
}

size_t WriteLE32_exc(std::ostream &dio,
        uint32_t i)
{
    i = clc::ByteOrder::swapUInt32OnLE(i);
    return Write_exc(dio, (char const *)&i, 4);
}

size_t WriteBE16_exc(std::ostream &dio,
        uint16_t i)
{
    i = clc::ByteOrder::swapUInt16OnLE(i);
    return Write_exc(dio, (char const *)&i, 2);
}

size_t WriteBE32_exc(std::ostream &dio,
        uint32_t i)
{
    i = clc::ByteOrder::swapUInt32OnLE(i);
    return Write_exc(dio, (char const *)&i, 4);
}

size_t WriteCStr_exc(std::ostream &dio,
        clc::Buffer const &src)
{
    size_t len = src.length() + 1;

    Write_exc(dio, src.c_str(), len);
    return len;
}

size_t WriteCStr_exc(std::ostream &dio,
        char const *src)
{
    size_t len = strlen(src) + 1;

    Write_exc(dio, src, len);
    return len;
}

size_t Read_exc(std::istream &dio,
        char *dst,
        size_t size)
{
    if (dio.read(dst, size).fail())
        throw clc::IOException();
    return size;
}

size_t Read8_exc(std::istream &dio,
        uint8_t &i)
{
    return Read_exc(dio, (char *)&i, 1);
}

size_t ReadLE16_exc(std::istream &dio,
        uint16_t &i)
{
    size_t bytes = Read_exc(dio, (char *)&i, 2);

    if (bytes != 2)
        throw clc::IOException();
    i = clc::ByteOrder::swapUInt16OnLE(i);
    return bytes;
}

size_t ReadLE32_exc(std::istream &dio,
        uint32_t &i)
{
    size_t bytes = Read_exc(dio, (char *)&i, 4);

    if (bytes != 4)
        throw clc::IOException();
    i = clc::ByteOrder::swapUInt32OnLE(i);
    return bytes;
}

size_t ReadBE16_exc(std::istream &dio,
        uint16_t &i)
{
    size_t bytes = Read_exc(dio, (char *)&i, 2);

    if (bytes != 2)
        throw clc::IOException();
    i = clc::ByteOrder::swapUInt16OnLE(i);
    return bytes;
}

size_t ReadBE32_exc(std::istream &dio,
        uint32_t &i)
{
    size_t bytes = Read_exc(dio, (char *)&i, 4);

    if (bytes != 4)
        throw clc::IOException();
    i = clc::ByteOrder::swapUInt32OnLE(i);
    return bytes;
}

size_t ReadCStr_exc(std::istream &dio,
        clc::Buffer &dst,
        unsigned int max)
{
    for (unsigned int i = 0; !max || i <= max; ++i) {
        char c;
        if (dio.read(&c, 1).fail())
            throw clc::IOException();
        if (c == 0)
            return i;
        dst.append(c, 1);
    }
    throw clc::IllegalFormatException();
}

size_t ReadCStr_exc(std::istream &dio,
        char *dst,
        unsigned int max)
{
    for (unsigned int i = 0; !max || i <= max; ++i) {
        char c;
        if (dio.read(&c, 1).fail())
            throw clc::IOException();
        dst[i] = c;
        if (c == 0)
            return i;
    }
    throw clc::IllegalFormatException();
}

#if 0
size_t ReadPStr_exc(std::istream &dio,
        clc::Buffer &dst,
        unsigned int max)
{
    uint8_t len;

    Read8_exc(dio, len);

#if 0  // TODO:  Would be less copying, but data returns const.
    dst.reserve(len);
    char *buffer = dst.data();
    size_t bytes = dio.read(buffer, len).fail();
#endif
    char tmp[len];
    if (dio.read(tmp, len).fail())
        throw clc::IOException();
    if (max && len > max)
        throw IllegalFormatException();
    dst.assign(len, tmp);
    return len + 1;
}

#endif

off_t GetRemaining(std::istream &dio)
{
    off_t current, end;

    current = dio.tellg();
    end = dio.seekg(0, std::ios::end).tellg();
    dio.seekg(current, std::ios::beg);
    assert(current <= end);
    return end - current;
}
