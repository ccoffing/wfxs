#ifndef XS_DATA_IO_H
#define XS_DATA_IO_H

/** @file
 *  @brief Convenience functions to read/write to streams.
 */

#include <iostream>
#include <sys/types.h>

/** Writes the data to the stream.
 *
 *  Pascal strings are written with a single byte length prefixed, then
 *  the string (maximum of 255 bytes (not characters)), and a terminating
 *  NULL byte.
 *
 *  On success, returns the actual number of bytes written (including any
 *  serialization overhead).  For most calls, the return matches the requested
 *  number of bytes.  For CStr, this is +1 due to the terminating NULL.  For
 *  PStr, this is +1 due to the prefixed length byte.  Partial writes are
 *  considered an IO error.
 *
 *  Throws MTIOExc on IO failures;
 *         MTSerializeExc if data is of wrong form (namely, if string is
 *         > 255 bytes when writing PStr).
 */
size_t Write_exc(std::ostream& pio, char const* src, size_t size);
size_t Write8_exc(std::ostream& pio, uint8_t i);
size_t WriteLE16_exc(std::ostream& pio, uint16_t i);
size_t WriteLE32_exc(std::ostream& pio, uint32_t i);
size_t WriteBE16_exc(std::ostream& pio, uint16_t i);
size_t WriteBE32_exc(std::ostream& pio, uint32_t i);
size_t WriteCStr_exc(std::ostream& pio, std::string const& src);
size_t WriteCStr_exc(std::ostream& pio, char const* src);
size_t WritePStr_exc(std::ostream& dio, std::string const& src);
size_t WritePStr_exc(std::ostream& dio, char const* src);

/** Reads data from the stream.
 *
 *  On success, the calls return the number of bytes actually read (including
 *  any potential serialization overhead).  Read_exc may return any number
 *  [1 .. size].  ReadCStr returns +1 due to the terminating NULL; ReadPStr
 *  returns +1 due to the length byte.  (Reading only a partial string is
 *  considered a failure.)  Other Read* calls will return the exact number of
 *  bytes requested.
 *
 *  Some Read* calls may be passed a maximum.  This is the maximum value
 *  expected (ignoring any serialization overhead, such as a length byte or
 *  terminating NULL).  This requires that the data fits within the maximum;
 *  if data would have to be truncated to fit, IllegalFormatException will be thrown.
 *
 *  WARNING:  The PStr and CStr calls store the resulting string as a NULL-
 *            terminated C string.  If a static buffer is used, max+1 bytes
 *            must be allocated.
 *
 *  Throws MTIOExc on IO failures;
 *         IllegalFormatException if data is of wrong form (namely, if maximums
 *         are exceeded).
 */
size_t Read_exc(std::istream& pio, char* dst, size_t size);
size_t Read8_exc(std::istream& pio, uint8_t& i);
size_t ReadLE16_exc(std::istream& pio, uint16_t& i);
size_t ReadLE32_exc(std::istream& pio, uint32_t& i);
size_t ReadBE16_exc(std::istream& pio, uint16_t& i);
size_t ReadBE32_exc(std::istream& pio, uint32_t& i);
size_t ReadCStr_exc(std::istream& pio, std::string& dst, unsigned int max = 0);
size_t ReadCStr_exc(std::istream& pio, char* dst, unsigned int max = 0);
size_t ReadPStr_exc(std::istream& dio, std::string& src, unsigned int max = 0);
size_t ReadPStr_exc(std::istream& dio, char* dst, unsigned int max = 0);

/**
 */
off_t GetRemaining(std::istream& pio);

#endif
