#include "XSFile.h"
#include "XSApplication.h"
#include "XSController.h"
#include "XSDataIO.h"
#include "XSException.h"
#include "XSFileNative.h"
#include "XSFloss.h"
#include "XSSkein.h"
#include "XSSquareIO.h"

#include <assert.h>
#include <iostream>
#include <string>


static unsigned int g_version = 1;

static void StreamOutNativeSquare(
        std::ostream& stream, XSSquareIO* square, unsigned int repeatCount)
{
    assert(repeatCount > 0 && repeatCount < 0x80);

    if (repeatCount != 1)
        Write8_exc(stream, repeatCount | 0x80);
    square->Serialize(stream);
}

void StreamOutNative(std::ostream& stream, const XSModel* model)
{
    XSToolState const toolState = model->ToolState();
    unsigned int squaresX = model->SquaresX();
    unsigned int squaresY = model->SquaresY();
    unsigned int layers = model->GetNumberLayers();

    Write8_exc(stream, 'w');
    Write8_exc(stream, 'f');
    Write8_exc(stream, 'x');
    Write8_exc(stream, 's');
    WriteLE32_exc(stream, g_version);
    WriteLE32_exc(stream, squaresX);
    WriteLE32_exc(stream, squaresY);
    WriteLE32_exc(stream, layers);

    /*
     *  Dump out meta-data
     */
    model->GetProperties().Serialize(stream);

    /*
     *  Write out the makers.  Only write out those that this document
     *  actually uses, to keep the program's and the document's lists
     *  of makers independent.
     *
     *  Disk format:
     *  1                  number of makers
     *  for each maker:
     *    n                index of this maker, as used later in the file
     *    strlen(maker)+1  CStr of maker's name
     */
    std::vector<unsigned int> makers;
    makers.reserve(toolState.m_makerNames.size());
    for (unsigned int i = 0; i < toolState.m_makerNames.size(); ++i) {
        makers.push_back(0);
    }
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
    toolState.m_flossPalette.Serialize(stream);

    /*
     *  Dump pattern data
     */
    for (unsigned int layer = 0; layer < layers; ++layer) {
        unsigned int repeatPreviousSquare = 0;
        XSSquareIO square, previousSquare;

        for (unsigned int y = 0; y < squaresY; ++y) {
            for (unsigned int x = 0; x < squaresX; ++x) {
                model->GetSquareData(&square, x, y, layer);

                if (repeatPreviousSquare == 0) {
                    previousSquare = square;
                    ++repeatPreviousSquare;
                } else {
                    assert(repeatPreviousSquare < 128);
                    if (repeatPreviousSquare < 127
                            && memcmp(&previousSquare, &square, sizeof(XSSquareIO)) == 0) {
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
    do {
        uint32_t u32, squaresX, squaresY, layers;

        ReadLE32_exc(stream, u32);
        // if (u32 != XSApplication::signature)
        //    break;

        ReadLE32_exc(stream, u32);
        if (u32 > g_version)
            break;

        ReadLE32_exc(stream, squaresX);
        ReadLE32_exc(stream, squaresY);
        ReadLE32_exc(stream, layers);
        if (!squaresX || !squaresY || !layers)
            break;

        XSProperties properties;
        properties.Unserialize(stream);
        doc->SetProperties(properties);

        uint8_t numMakers;
        Read8_exc(stream, numMakers);
        for (unsigned int i = 0; i < numMakers; ++i) {
            uint8_t index;
            Read8_exc(stream, index);
            std::string maker;
            ReadCStr_exc(stream, maker);
            // FIXME - add maker to set
        }

        // product lines

        // skeins

        XSToolState toolState = doc->m_toolState;
        toolState.m_flossPalette.Unserialize(stream);

        for (unsigned int layer = 0; layer < layers; ++layer) {
            uint8_t repeat = 0;
            XSSquareIO square;

            if (layer > 0)
                doc->AddLayer();

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
                        square.Unserialize(stream);
                    }
                    assert(repeat);
                    doc->SetSquareDataNoInval(&square, x, y, layer);
                    repeat--;
                }
            }

            assert(repeat == 0);
        }

        return;
    } while (false);

    throw IllegalFormatException();
}
