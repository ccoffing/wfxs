#include "XSApplication.h"
#include "XSData.h"
#include "XSToolState.h"


XSToolState::XSToolState() :
    m_toolType(ToolType_Stitch),
    m_tool(Tool_FullStitch),
    m_flossPalette(*makerFlossPalette[FM_DMC]),
    // TODO: xs_app->Prefs().GetPreferredMaker()]),
    m_flossIndex(0),
    m_makerNames(makerNames),
    m_productLineNames(productLineNames),
    m_beadIndex(0),
    m_stitchType(Stitch_Full),
    m_stitchTypeContinued(Stitch_Full),
    m_knotType(Knot_French),
    m_overwrite(false)
{
}
