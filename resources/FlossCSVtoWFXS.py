#!/usr/bin/env python
"""
Usage:
$0 <vendor> [beos|c|bin]
"""

import sys
import struct

def usage():
    print __doc__
    sys.exit(1)

def printf(fmt, *args):
    sys.stdout.write(fmt % args)

format = 'bin'
if len(sys.argv) > 2:
    if sys.argv[2] not in ('c', 'bin', 'beos'):
        usage()
    format = sys.argv[2]

_enum_vendor = {
    'dmc': 0,
    'anchor': 1,
    'jpcoats': 2,
    'kreinik': 3,
    'millhill': 4
}

enum_vendor = {
    'DMC': 0,
    'Anchor': 1,
    'JP Coats': 2,
    'Kreinik': 3,
    'Mill Hill': 4
}

enum_product = {
    'Emb'       : 0,
    'Rayon'     : 1,
    'Linen'     : 2,
    'Filament'  : 3,
    'Light'     : 4,
    'Metallic'  : 5,
    'Pastelles' : 6,
    'Variations': 7,
    'Braid'     : 8,
    'Ribbon8'   : 9,
    'Ribbon16'  : 10,
    'Cord'      : 11,
    'Facets'    : 12,
    'Japan'     : 13,
}

enum_blend = {
    'None': 0,
    'Smooth': 1,
    'Banded': 2
}

def dequote(s):
    if len(s) > 1 and s[0] == '"' and s[len(s) - 1] == '"':
        return s[1:len(s)-1]
    return s

class Contents:
    def __init__(self):
        pass
    def flush(self):
        pass
    def add(self, chunk):
        pass
contents = Contents()

class Chunk:
    def __init__(self, name):
        self.data = []
        self.name = name
        self.n = 0

    def flush(self):
        if format == 'c':
            printf('char const * %s[] = {\n', self.name)
            for line in self.data:
                if line:
                    printf('    ')
                    for byte in line:
                        printf('0x%02x, ', byte)
                    printf('\n')
            printf('};\n')
        elif format == 'beos':
            printf('resource(%d, "%s") array {\n', _enum_vendor[sys.argv[1]]*2, self.name)
            for line in self.data:
                if line:
                    printf('    $"')
                    for byte in line:
                        printf('%02x', byte)
                    printf('"\n')
            printf('};\n')
        else:
            printf('%s', struct.pack('!h', self.n))
            for byte in self.data:
                printf('%c', byte)

        contents.add(self)

    def append_byte(self, byte):
        self.data.append(byte)

    def append_bytes(self, bytes):
        for byte in bytes:
           self.append_byte(byte)

    def newline(self):
        self.n = self.n + 1

filename = sys.argv[1] + "_sp"
chunk = Chunk(filename)
lines = open("resources/floss/" + filename + ".csv", 'r').readlines()
lines = lines[1:]  # discard header
count = 0
for line in lines:
    count += 1
    floss = line.rstrip().split(',')
    vendor  = dequote(floss[0])
    product = dequote(floss[1])
    id      = dequote(floss[2])
    desc    = dequote(floss[3])
    color   = dequote(floss[4])
    discontinued = (dequote(floss[5]) == '1')

    blend = 'None'
    colors = color.split(' ')
    if len(colors) > 1:
        blend = 'Banded'
    else:
        colors = color.split('-')  # variagated
        if len(colors) > 1:
            blend = 'Smooth'
        else:
            colors = [ color ]

    flags = 0
    if discontinued:
        flags |= 1;
    flags |= (enum_blend[blend] << 1);

    # Format is:
    # 2: total length
    # 1: Vendor
    # 1: Product line
    # Null-terminated ID string
    # Null-terminated description string
    # 1: bit-field:
    #  bit 0:  discontinued?
    #  bits 1-2:  how to blend the colors
    # 1: # colors
    # 3: each color

    chunk.append_bytes((enum_vendor[vendor], enum_product[product]))
    for c in id:
        chunk.append_byte(ord(c))
    chunk.append_byte(0)
    for c in desc:
        chunk.append_byte(ord(c))
    chunk.append_bytes((0, flags, len(colors)))
    for color in colors:
        if len(color) != 6:
            raise "Color does not look like a hex code: '%s'" % (color)
        color = color.lower();
        chunk.append_bytes((int(color[0:2], 16), int(color[2:4], 16), int(color[4:6], 16)))

    chunk.newline()
chunk.flush()

filename = sys.argv[1] + "_fp"
chunk = Chunk(filename)
lines = open("resources/floss/" + filename + ".csv", 'r').readlines()
lines = lines[1:]  # discard header
count = 0
for line in lines:
    count += 1
    skein = line.rstrip().split(',')
    vendor     = dequote(skein[0])
    prodline   = dequote(skein[1])
    id         = dequote(skein[2])
    strands    = int(dequote(skein[3]))
    symbol     = dequote(skein[4])
    # TODO: for now, always 1
    num_skeins = 1

    chunk.append_bytes((num_skeins, strands, enum_vendor[vendor], enum_product[prodline]))
    for c in id:
        chunk.append_byte(ord(c))
    chunk.append_byte(0)
    chunk.append_byte(ord(symbol))
    chunk.append_byte(0)
    chunk.newline()
chunk.flush()

contents.flush()
