#include "Logger.h"
#include "XSFile.h"
#include "XSApplication.h"
#include "XSController.h"
#include "XSDataIO.h"
#include "XSException.h"
#include "XSFileNative.h"
#include "XSFloss.h"
#include "XSSkein.h"
#include "XSSquareIO.h"

#include <cassert>
#include <iostream>
#include <string.h>
#include <string>

#define LOG_NAME "xs.FileNative"

static unsigned int g_version = 1;

static void StreamOutNativeSquare(
        std::ostream& stream, XSSquareIO* square, unsigned int repeatCount)
{
    assert(repeatCount > 0 && repeatCount < 0x80);

    if (repeatCount != 1)
        Write8_exc(stream, repeatCount | 0x80);
    square->serialize(stream);
}

void StreamOutNative(std::ostream& stream, const XSModel* model)
{
    XSToolState const toolState = model->toolState();
    unsigned int squaresX = model->squaresX();
    unsigned int squaresY = model->squaresY();
    unsigned int layers = model->getNumberLayers();

    Write8_exc(stream, 'w');
    Write8_exc(stream, 'f');
    Write8_exc(stream, 'x');
    Write8_exc(stream, 's');
    WriteBE32_exc(stream, g_version);
    WriteBE32_exc(stream, squaresX);
    WriteBE32_exc(stream, squaresY);
    WriteBE32_exc(stream, layers);

    // Dump out meta-data
    model->getProperties().serialize(stream);

    //  Write out the makers.  Only write out those that this document
    //  actually uses, to keep the program's and the document's lists
    //  of makers independent.
    //
    //  Disk format:
    //  1                  number of makers
    //  for each maker:
    //    n                index of this maker, as used later in the file
    //    strlen(maker)+1  CStr of maker's name
    std::vector<unsigned int> makers(toolState.m_makerNames.size());
    // Mark all that are used
    unsigned int used = 0;
    for (unsigned int i = 0; i < toolState.m_flossPalette.size(); ++i) {
        XSFloss const& floss = toolState.m_flossPalette[i];
        for (unsigned int j = 0; j < floss.m_numskeins; ++j) {
            if (makers[floss.m_skeins[j]->m_maker] == 0) {
                makers[floss.m_skeins[j]->m_maker] = 1;
                used++;
            }
        }
    }
    // Write out those that are actually used, and re-number them for
    // use by the pattern as saved on disk.
    Write8_exc(stream, used);
    used = 0;
    for (unsigned int i = 0; i < makers.size(); ++i) {
        if (makers[i]) {
            Write8_exc(stream, used);
            WriteCStr_exc(stream, toolState.m_makerNames[i]);
            makers[i] = used++;
        }
    }

    /*
     *  Dump product lines
     */

    /*
     *  Dump skeins
     */
    // FIXME:  only save out skeins used by the palette

    /*
     *  Dump floss
     *
     *  Unlike the makers and product lines (above), write out all entries
     *  in the floss palette.  The user set these up, so assume there's
     *  a reason, even if they're not used (yet!) in the pattern.
     */
    // FIXME:  pass "makers" down as a way to re-number the output
    toolState.m_flossPalette.serialize(stream);

    // Dump pattern data
    for (unsigned int layer = 0; layer < layers; ++layer) {
        unsigned int repeatPreviousSquare = 0;
        XSSquareIO square, previousSquare;

        for (unsigned int y = 0; y < squaresY; ++y) {
            for (unsigned int x = 0; x < squaresX; ++x) {
                model->getSquareData(&square, x, y, layer);

                if (repeatPreviousSquare == 0) {
                    previousSquare = square;
                    ++repeatPreviousSquare;
                } else {
                    assert(repeatPreviousSquare < 128);
                    if (repeatPreviousSquare < 127
                            && memcmp(&previousSquare, &square, sizeof(XSSquareIO)) == 0) {  // XXX operator=
                        ++repeatPreviousSquare;
                    } else {
                        // They differ, or the previous run hit the max length.
                        // Now the previous run ends, and a new one starts.
                        StreamOutNativeSquare(stream, &previousSquare, repeatPreviousSquare);
                        previousSquare = square;
                        repeatPreviousSquare = 1;
                    }
                }
            }
        }
        StreamOutNativeSquare(stream, &previousSquare, repeatPreviousSquare);

        // FIXME:  save backstitch for layer
    }
}

void StreamInNative(std::istream& stream, XSModel* doc)
{
    uint32_t u32, squaresX, squaresY, layers;

    ReadBE32_exc(stream, u32);
    if (u32 != 'wfxs') {
        throw IllegalFormatException("Unrecognized file signature");
    }

    ReadBE32_exc(stream, u32);
    if (u32 > g_version) {
        throw IllegalFormatException("Unrecognized file version");
    }
    Log::debug(LOG_NAME, "File version 0x%x", u32);

    ReadBE32_exc(stream, squaresX);
    ReadBE32_exc(stream, squaresY);
    ReadBE32_exc(stream, layers);
    if (!squaresX || !squaresY || !layers) {
        throw IllegalFormatException("Pattern has a 0 dimension");
    }
    Log::debug(LOG_NAME, "Pattern is %ux%ux%u", squaresX, squaresY, layers);

    XSProperties properties;
    properties.unserialize(stream);
    doc->setProperties(properties);
    doc->resize(squaresX, squaresY);

    uint8_t numMakers;
    Read8_exc(stream, numMakers);
    Log::debug(LOG_NAME, "Pattern uses %u makers", numMakers);
    for (unsigned int i = 0; i < numMakers; ++i) {
        uint8_t index;
        Read8_exc(stream, index);
        std::string maker;
        ReadCStr_exc(stream, maker);
        Log::debug(LOG_NAME, "Pattern uses maker '%s'", maker.c_str());
        // FIXME - add maker to set
    }

    // product lines

    // skeins

    XSToolState toolState = doc->m_toolState;
    toolState.m_flossPalette.unserialize(stream);

    for (unsigned int layer = 0; layer < layers; ++layer) {
        uint8_t repeat = 0;
        XSSquareIO square;

        if (layer > 0) {
            Log::debug(LOG_NAME, "Adding another layer");
            doc->addLayer();
        }

        for (unsigned int y = 0; y < squaresY; ++y) {
            for (unsigned int x = 0; x < squaresX; ++x) {
                if (repeat == 0) {
                    Read8_exc(stream, repeat);
                    if (repeat & 0x80) {
                        repeat &= 0x7f;
                    } else {
                        stream.seekg(-1, std::ios::cur);
                        repeat = 1;
                    }
                    square.unserialize(stream);
                    Log::debug(LOG_NAME, "...repeats %u", repeat);
                }
                assert(repeat > 0);
                doc->setSquareDataNoInval(&square, x, y, layer);
                repeat--;
            }
        }

        assert(repeat == 0);
    }
}
