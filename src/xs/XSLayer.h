#ifndef XS_LAYER_H
#define XS_LAYER_H

#include "XSStitchTypes.h"

#include <map>

class XSSquare;
struct XSSquareIO;


/**
 *  A layer of a cross-stitch pattern.
 */
class XSLayer {
public:
    XSLayer(unsigned int sizeX, unsigned int sizeY);
    XSLayer(const XSLayer&);
    ~XSLayer();

    /**
     *  Resizes the layer to the specified size.
     *  If the size is larger than the existing size, the new squares are left empty.
     *  If the size is smaller, existing squares are simply deleted.
     *  The pictures remains rooted at the same origin.
     */
    void Resize(unsigned int sizeX, unsigned int sizeY);

    StitchType SetStitch(unsigned int squareX, unsigned int squareY, unsigned int xPercent,
            unsigned int yPercent, StitchType stitchType, unsigned int flossIndex, bool overwrite);

    void SetKnot(unsigned int x, unsigned int y, unsigned int region, KnotType knotType,
            unsigned int flossIndex, bool overwrite);

    void SetBead(unsigned int x, unsigned int y, unsigned int region, unsigned int colorIndex,
            bool overwrite);

    void ClearSquare(unsigned int squareX, unsigned int squareY);

    void SetSquareData(XSSquareIO const* square, unsigned int x, unsigned int y);

    void GetSquareData(XSSquareIO* square, unsigned int squareX, unsigned int squareY);

    void Backstitch();

    unsigned int getX() const
    {
        return m_sizeX;
    }

    unsigned int getY() const
    {
        return m_sizeY;
    }

    XSSquare* getSquare(unsigned int x, unsigned int y) const;

protected:
    StitchTypeAndLocation autoStitch(unsigned int squareX, unsigned int squareY,
            unsigned int xPercent, unsigned int yPercent, StitchType stitchType,
            unsigned int flossIndex);

    void replaceStitch(unsigned int squareX, unsigned int squareY, StitchTypeAndLocation stitchType,
            unsigned int flossIndex, bool overwrite);

    void SetEmbelishment(EmbelType type, unsigned int x, unsigned int y, unsigned int region,
            KnotType knotType, unsigned int index, bool overwrite);

    /*
     *  Returns true if and only if the two stitches are of the same type.
     *  Only the type of stitch (full, half, etc) is considered, not orientation.
     */
    bool sameType(StitchType s1, StitchType s2);

private:
    // Unimplemented; disallowed.
    XSLayer& operator=(XSLayer const&);

    unsigned int m_sizeX;
    unsigned int m_sizeY;

    XSSquare* m_grid;

    struct EmbLocation {
        unsigned int x;
        unsigned int y;
        unsigned int region;
        bool operator<(struct EmbLocation const& B) const
        {
            return x < B.x || y < B.y || region < B.region;
        }
    };
    struct Embelishment {
        EmbelType m_embType;
        union {
            struct {
                KnotType m_knotType;
                unsigned int m_flossIndex;
            } knot;
            struct {
                unsigned int m_colorIndex;
            } bead;
        };
    };
    std::map<EmbLocation, Embelishment> m_embelishments;
};

#endif
