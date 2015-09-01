#ifndef XS_TOOL_STATE_H
#define XS_TOOL_STATE_H

#include "XSFlossPalette.h"
#include "XSStitchTypes.h"
#include "XSToolTypes.h"

#include <vector>

/**
 */
class XSToolState {
public:
    XSToolState();

    ToolType m_toolType;
    Tool m_tool;
    XSFlossPalette m_flossPalette;
    unsigned int m_flossIndex;
    std::vector<std::string> m_makerNames;
    std::vector<std::string> m_productLineNames;
    unsigned int m_beadIndex;
    StitchType m_stitchType;
    StitchType m_stitchTypeContinued;
    KnotType m_knotType;
    bool m_overwrite;
};

#endif
