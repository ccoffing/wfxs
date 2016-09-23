#ifndef XS_TOOL_TYPES_H
#define XS_TOOL_TYPES_H


typedef enum _Tool {
    Tool_Backstitch,
    Tool_Bead,
    Tool_ColorPicker,
    Tool_Couching,
    Tool_Eraser,
    Tool_FloodFill,
    Tool_FreeFormSelect,
    Tool_FrenchKnot,
    Tool_FullStitch,
    Tool_HalfAutoStitch,
    Tool_HalfBottomStitch,
    Tool_HalfTopStitch,
    Tool_LazyDaisyKnot,
    Tool_QuarterAutoStitch,
    Tool_QuarterLLStitch,
    Tool_QuarterLRStitch,
    Tool_QuarterULStitch,
    Tool_QuarterURStitch,
    Tool_RectangularSelect,
    Tool_SpecialStitch,  // FIXME:  enumerate these?
    Tool_Text,
    Tool_ThreeQuarterAutoStitch,
    Tool_ThreeQuarterLLStitch,
    Tool_ThreeQuarterLRStitch,
    Tool_ThreeQuarterULStitch,
    Tool_ThreeQuarterURStitch,
} Tool;

typedef enum _ToolType {
    ToolType_Backstitch,
    ToolType_Bead,
    ToolType_ColorPicker,
    ToolType_Couching,
    ToolType_Eraser,
    ToolType_FloodFill,
    ToolType_Knot,
    ToolType_Select,
    ToolType_Stitch,
    ToolType_Text,
} ToolType;

inline bool ToolTypeUsesCursor(ToolType toolType)
{
    return toolType != ToolType_Couching && toolType != ToolType_Backstitch
            && toolType != ToolType_Text && toolType != ToolType_Select;
}

#endif
