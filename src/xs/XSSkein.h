#ifndef XS_SKEIN_H
#define XS_SKEIN_H

/** @file
 *  Represents a particular skein of floss (maker, color, strands, etc).
 */

#include <iostream>
#include <vector>

#include "clc/data/Buffer.h"

#include "XSColor.h"


// FIXME:  move this to XSData.h
typedef enum {
    FM_DMC = 0,
    FM_ANCHOR,
    FM_JPCOATS,
    FM_KREINIK,
    FM_MILLHILL,
    FM_NUMBER
} Maker_t;

typedef enum {
    FPL_EMBROIDERY = 0,     // Embroidery Floss
    FPL_RAYON,              // Rayon Embroidery Floss
    FPL_LINEN,              // Linen Embroidery Floss
    FPL_BLENDING_FILAMENT,  // Blending Filament
    FPL_LIGHT_EFFECTS,      // Light Effects
    FPL_METALLIC,           // Metallic Embroidery Floss
    FPL_PASTELLES,          //
    FPL_VARIATIONS,         // Color Variations Embroidery Floss
    FPL_BRAID,              // Braid
    FPL_RIBBON8,            // 1/8" Ribbon
    FPL_RIBBON16,           // 1/16" Ribbon
    FPL_CORD,               // Cord
    FPL_FACETS,             // Facets
    FPL_JAPAN,              // Japan Thread
    FPL_NUMBER
} FlossProductLine_t;

typedef enum {
    FB_NONE = 0,
    FB_SMOOTH,
    FB_BANDED,
    FB_NUMBER
} FlossBlend_t;

#define MAX_FLOSS_ID_LEN    20
#define MAX_FLOSS_DESC_LEN  50

/**
 *  Represents a particular skein of floss (maker, color, strands, etc).
 */
class XSSkein
{
public:
    XSSkein();
    virtual ~XSSkein();
    XSSkein(std::istream &src);
    XSSkein(Maker_t maker,
            FlossProductLine_t productLine,
            const char *id,
            const char *description,
            XSColor color,
            bool discontinued,
            FlossBlend_t blend);
    XSSkein(XSSkein const &ref);
    XSSkein &operator=(XSSkein const &rhs);
    //  bool operator==(XSSkein const& rhs) const;

    void Serialize(std::ostream &file) const;
    void Unserialize(std::istream &src);
    void SerializeRef(std::ostream &file) const;
    static void UnserializeRef(std::istream &src, Maker_t &maker,
            FlossProductLine_t &productLine, clc::Buffer &id);

    // Identifying tuple:
    Maker_t m_maker;
    FlossProductLine_t m_productLine;
    clc::Buffer m_id;

    clc::Buffer m_description;
    std::vector<XSColor> m_colors;
    bool m_discontinued;
    FlossBlend_t m_blend;
};

#endif
