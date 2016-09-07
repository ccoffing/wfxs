#include "Logger.h"
#include "XSApplication.h"
#include "XSDataIO.h"
#include "XSFlossPalette.h"
#include "XSSkein.h"
#include "XSSkeinPalette.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>

#define LOG_NAME "xs.Data"


XSSkeinPalette *makerSkeinPalette[FM_NUMBER];
XSFlossPalette *makerFlossPalette[FM_NUMBER];

// All makers known to the program.  This is the base set that a pattern
// copies from to start with, but the pattern can come with its own, too.
std::vector<std::string> makerNames;
static char const *makerNamesCStr[] =
{
    "DMC",
    "Anchor",
    "JP Coats",
    "Kreinik",
    "Mill Hill"
};

std::vector<std::string> productLineNames;
static char const *productLineNamesCStr[] =
{
    "Embroidery Floss",
    "Rayon Embroidery Floss",
    "Linen Embroidery Floss",
    "Blending Filament",
    "Light Effects",
    "Metallic Embroidery Floss",
    "Pastelles",
    "Color Variations Embroidery Floss",
    "Braid",
    "1/8\" Ribbon",
    "1/16\" Ribbon",
    "Cord",
    "Facets",
    "Japan Thread"
};


static void InitSkeinPalettes()
{
    struct {
        const char *name;
        Maker_t index;
    } toRead[] = {
        { "dmc",     FM_DMC     },
        { "anchor",  FM_ANCHOR  },
        { "jpcoats", FM_JPCOATS },
        { 0,         FM_NUMBER  }
    };

#if 1
    // FIXME:  fix Makefile to package up things in a testable manner -- appdir, ...
    std::string rsrc = "resources";
#else
    std::string rsrc = wxStandardPaths::Get().GetResourcesDir();
#endif
    for (unsigned int i = 0; toRead[i].name; ++i) {
        std::string f = rsrc;
        f += "/floss/";
        f += toRead[i].name;
        Log::info(LOG_NAME, "Loading %s", f.c_str());
        std::ifstream is;
        is.open(f.c_str());

        Log::debug(LOG_NAME, "Loading skeins");
        XSSkeinPalette *sp = new XSSkeinPalette;
        sp->Unserialize(is);
        makerSkeinPalette[toRead[i].index] = sp;

        Log::debug(LOG_NAME, "Loading floss palette");
        XSFlossPalette *fp = new XSFlossPalette;
        fp->Unserialize(is);
        makerFlossPalette[toRead[i].index] = fp;
    }

    // FIXME:  init user's custom skeins
    // FIXME:  init user's custom palette
}

static void InitMakers()
{
    Log::info(LOG_NAME, "Loading makers");

    for (unsigned int i = 0; i < FM_NUMBER; ++i) {
        makerNames.push_back(std::string(makerNamesCStr[i]));
    }
}

static void InitProductLines()
{
    Log::info(LOG_NAME, "Loading product lines");

    for (unsigned int i = 0; i < FPL_NUMBER; ++i) {
        productLineNames.push_back(std::string(productLineNamesCStr[i]));
    }
}

void InitXSData()
{
    InitSkeinPalettes();
    InitMakers();
    InitProductLines();
}
