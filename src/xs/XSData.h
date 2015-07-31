#ifndef XS_DATA_H
#define XS_DATA_H

/** @file
 *  @brief Data: skeins, palettes, floss makers, product lines
 */

#include <vector>

#include "XSSkein.h"

class XSFlossPalette;
class XSSkeinPalette;

extern XSSkeinPalette *makerSkeinPalette[FM_NUMBER];
extern XSFlossPalette *makerFlossPalette[FM_NUMBER];
extern std::vector<clc::Buffer> makerNames;
extern std::vector<clc::Buffer> productLineNames;

/** Initializes data (skeins, palettes, etc) at program startup.
 */
void InitXSData();

#endif
