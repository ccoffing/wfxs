#ifndef XS_DATA_H
#define XS_DATA_H

/** @file
 *  @brief Data: skeins, palettes, floss makers, product lines
 */

#include "XSSkein.h"

#include <vector>

class XSFlossPalette;
class XSSkeinPalette;

extern XSSkeinPalette* makerSkeinPalette[FM_NUMBER];
extern XSFlossPalette* makerFlossPalette[FM_NUMBER];
extern std::vector<std::string> makerNames;
extern std::vector<std::string> productLineNames;

/** Initializes data (skeins, palettes, etc) at program startup.
 */
void InitXSData();

#endif
