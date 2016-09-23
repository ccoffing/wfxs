#include "XSWindowMenuUpdater.h"
#include "XSCommand.h"
#include "XSController.h"
#include "XSMessages.h"
#include <BAF/app/Messages.h>
#include <mt/app/Command.h>

XSWindowMenuUpdater::XSWindowMenuUpdater(XSController* controller)
    : m_controller(controller)
{
}

bool XSWindowMenuUpdater::IsEnabled(uint32_t what) const
{
    switch (what) {
    case BAF_MSG_REVERT_TO_SAVED: {
        if (!m_controller->CommandStack()->IsModified())
            return false;

        XSProperties const& properties = m_controller->GetModel()->GetProperties();
        return properties.m_filename.Length() != 0;
    }

    case BAF_MSG_UNDO:
        return m_controller->CommandStack()->CanUnDo();

    case BAF_MSG_REDO:
        return m_controller->CommandStack()->CanReDo();

    case BAF_MSG_H_CENTER:
    case BAF_MSG_V_CENTER:
    case BAF_MSG_CENTER:
    case BAF_MSG_CUT:
    case BAF_MSG_COPY:
        return false;  // FIXME:  anything selected?

    case BAF_MSG_PASTE:
        return false;  // FIXME:  anything in the clipboard?

    case BAF_MSG_ZOOM_IN:
    case BAF_MSG_ZOOM_OUT: {
        XSModel* model = m_controller->GetModel();
        unsigned int zoom = model->GetZoom();
        unsigned int min, max;
        model->GetZoomRange(&min, &max);
        return (what == BAF_MSG_ZOOM_IN) ? (zoom < max) : (zoom > min);
    }

    case XS_EVT_LAYER_DOWN:
    case XS_EVT_LAYER_UP:
        return m_controller->GetModel()->GetNumberLayers() > 1;
    }
    return true;
}

bool XSWindowMenuUpdater::IsMarked(uint32_t what) const
{
    switch (what) {
    case XS_EVT_PATTERN_STYLE:
        return m_controller->GetModel()->GetDrawStyle() == DrawStyle_Pattern;
    case XS_EVT_DESIGN_STYLE:
        return m_controller->GetModel()->GetDrawStyle() == DrawStyle_Design;
    case XS_EVT_REALISTIC_STYLE:
        return m_controller->GetModel()->GetDrawStyle() == DrawStyle_Realistic;
    case XS_EVT_SHOW_GRID:
        return m_controller->GetModel()->IsShowGrid();
    case XS_EVT_SHOW_RULERS:
        return m_controller->GetModel()->IsShowRulers();
    case XS_EVT_BEAD:
        return m_controller->GetModel()->GetToolType() == ToolType_Bead;
    case XS_EVT_FULL_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_Full;
    case XS_EVT_HALF_AUTO_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_HalfAuto;
    case XS_EVT_HALF_TOP_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_HalfTop;
    case XS_EVT_HALF_BOTTOM_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_HalfBottom;
    case XS_EVT_THREEQUARTER_UL_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_ThreeQuarterUL;
    case XS_EVT_THREEQUARTER_UR_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_ThreeQuarterUR;
    case XS_EVT_THREEQUARTER_LL_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_ThreeQuarterLL;
    case XS_EVT_THREEQUARTER_LR_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_ThreeQuarterLR;
    case XS_EVT_THREEQUARTER_AUTO_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_ThreeQuarterAuto;
    case XS_EVT_QUARTER_UL_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_QuarterUL;
    case XS_EVT_QUARTER_UR_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_QuarterUR;
    case XS_EVT_QUARTER_LL_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_QuarterLL;
    case XS_EVT_QUARTER_LR_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_QuarterLR;
    case XS_EVT_QUARTER_AUTO_STITCH:
        return m_controller->GetModel()->GetStitchType() == Stitch_QuarterAuto;
    case XS_EVT_FRENCH_KNOT:
        return m_controller->GetModel()->GetToolType() == ToolType_Knot;
    case BAF_MSG_OVERWRITE:
        return m_controller->ToolState().m_overwrite;
    }
    return false;
}

bool XSWindowMenuUpdater::GetLabel(uint32_t what, BString& label) const
{
    switch (what) {
    case BAF_MSG_UNDO:
    case BAF_MSG_REDO: {
        static char const* prevDescs[2];
        mt::Command const* mc;
        if (what == BAF_MSG_UNDO)
            mc = m_controller->CommandStack()->GetUnDoCommand();
        else
            mc = m_controller->CommandStack()->GetReDoCommand();
        char const* desc = 0;
        if (mc)
            desc = mc->GetDescription();
        if (desc != prevDescs[what == BAF_MSG_UNDO ? 0 : 1]) {
            // label.SetTo(what==BAF_MSG_UNDO ? "Undo" : "Redo");
            if (desc) {
                label += " ";
                label += desc;
            }
            prevDescs[what == BAF_MSG_UNDO ? 0 : 1] = desc;
            return true;
        }
        break;
    }
    }
    return false;
}
