#include "XSController.h"
#include "XSCommandClearSquare.h"
#include "XSCommandFloodFill.h"
#include "XSCommandLayer.h"
#include "XSCommandSetBead.h"
#include "XSCommandSetFlossPalette.h"
#include "XSCommandSetKnot.h"
#include "XSCommandSetProperties.h"
#include "XSCommandSetSize.h"
#include "XSCommandSetStitch.h"
#include "XSCommandSetStyle.h"
#include "XSCommandSetTool.h"
#include "XSCommandShowGrid.h"
#include "XSCommandShowRulers.h"
#include "XSCommandStack.h"
#include "XSCommandZoom.h"
#include "XSFileNative.h"
#include "XSFlossPalette.h"
#include "XSMessages.h"
#include "XSPoint.h"

#include <fstream>


XSController::XSController(XSModel* model)
    : m_commandStack(model)
    , m_model(model)
    , m_propertiesWindow(0)
    , m_editFlossWindow(0)
{
    m_model->setController(this);
}

void XSController::save(const XSModel& model, const char* filename)
{
    std::ofstream of;

    of.open(filename);
    model.saveObject(of);
}

void XSController::open(const char* filename)
{
    std::ifstream ifs;

    ifs.open(filename);

    // TODO
    XSModel* model = new XSModel(0, 0);
    model->loadObject(ifs);
    m_model = model;
}

void XSController::onPatternStyle(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandSetStyle(DrawStyle_Pattern));
}

void XSController::onDesignStyle(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandSetStyle(DrawStyle_Design));
}

void XSController::onRealisticStyle(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandSetStyle(DrawStyle_Realistic));
}

void XSController::onShowGrid(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandShowGrid(!m_model->isShowGrid()));
}

#if 0
void x()
{
    switch (0) {

    case XS_EVT_DIALOG_CLOSING:
    {
        int s;
        int32_t dialog, button;

        s = message->FindInt32("dialog", &dialog);
        assert(s == B_OK);
        s = message->FindInt32("button", &button);
        assert(s == B_OK);

        switch ((XSWindowDialogs)dialog) {
        case XS_DIALOG_PROPERTIES:
            if ((unsigned int)button == XS_EVT_BUTTON_OKAY) {
                m_commandStack.doCommand(new XSCommandSetProperties(m_propertiesWindow->m_properties));
            }
            m_propertiesWindow->PostMessage(XS_EVT_QUIT_REQUESTED);
            m_propertiesWindow = 0;
            break;
        case XS_DIALOG_EDIT_FLOSS:
            // FIXME -- retrieve settings
            m_editFlossWindow->PostMessage(XS_EVT_QUIT_REQUESTED);
            m_editFlossWindow = 0;
            break;
        default:
            assert(0);
        }
        break;
    }

    case XS_EVT_REVERT_TO_SAVED:
    {
        BAlert *alert = new BAlert(_("Revert to saved?"),
                _("Do you want to discard your changes, and revert to the previously saved version?"),
                _("Cancel"), _("Discard& Revert"), NULL,
                B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_WARNING_ALERT);
        alert->SetShortcut(0, B_ESCAPE);
        int32 button_index = alert->Go();
        if (button_index == 1) {
            printf("FIXME:  revert\n");
        }
        break;
    }

    case XS_EVT_EDIT_PALETTE:
        if (m_editFlossWindow)
            m_editFlossWindow->Show();
        else
            m_editFlossWindow = new XSEditFlossWindow(this, m_model->toolState().m_flossPalette);
        break;
    }
}

#endif

void XSController::onZoomIn(CommandEvent& event)
{
    unsigned int zoom = m_model->getZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom += increment;
    m_commandStack.doCommand(new XSCommandZoom(zoom));
}

void XSController::onZoomOut(CommandEvent& event)
{
    unsigned int zoom = m_model->getZoom();
    unsigned int increment = zoom / 8;

    if (!increment)
        ++increment;
    zoom -= increment;
    m_commandStack.doCommand(new XSCommandZoom(zoom));
}

void XSController::onOverwrite()
{
    m_model->toolState().m_overwrite = !m_model->toolState().m_overwrite;
}

void XSController::onFullStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_Full;
}

void XSController::onHalfAutoStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_HalfAuto;
}

void XSController::onHalfTopStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_HalfTop;
}

void XSController::onHalfBottomStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_HalfBottom;
}

void XSController::onThreeQuarterAutoStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_ThreeQuarterAuto;
}

void XSController::onThreeQuarterULStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_ThreeQuarterUL;
}

void XSController::onThreeQuarterURStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_ThreeQuarterUR;
}

void XSController::onThreeQuarterLLStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_ThreeQuarterLL;
}

void XSController::onThreeQuarterLRStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_ThreeQuarterLR;
}

void XSController::onQuarterAutoStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_QuarterAuto;
}

void XSController::onQuarterULStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_QuarterUL;
}

void XSController::onQuarterURStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_QuarterUR;
}

void XSController::onQuarterLLStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_QuarterLL;
}

void XSController::onQuarterLRStitch()
{
    m_model->toolState().m_toolType = ToolType_Stitch;
    m_model->toolState().m_stitchType = Stitch_QuarterLR;
}

void XSController::onLayerAdd(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandLayerAdd());
}

void XSController::onLayerDel(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandLayerDel());
}

void XSController::onLayerUp(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandLayerUp());
}

void XSController::onLayerDown(CommandEvent& event)
{
    m_commandStack.doCommand(new XSCommandLayerUp());
}

void XSController::onToolEraserSmall(CommandEvent& event)
{
    m_model->toolState().m_toolType = ToolType_Eraser;
}

void XSController::onToolEraserMedium(CommandEvent& event)
{
    m_model->toolState().m_toolType = ToolType_Eraser;
}

void XSController::onToolEraserLarge(CommandEvent& event)
{
    m_model->toolState().m_toolType = ToolType_Eraser;
}

void XSController::onToolFloodFill(CommandEvent& event)
{
    m_model->toolState().m_toolType = ToolType_FloodFill;
}

void XSController::onColorPicker(CommandEvent& event)
{
    m_model->toolState().m_toolType = ToolType_ColorPicker;
}

#if 0
void XSController::setTool(Tool tool)
{
    switch (tool) {
    case Tool_FullStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_Full;
        break;
    case Tool_ThreeQuarterAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterAuto;
        break;
    case Tool_ThreeQuarterULStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterUL;
        break;
    case Tool_ThreeQuarterURStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterUR;
        break;
    case Tool_ThreeQuarterLLStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterLL;
        break;
    case Tool_ThreeQuarterLRStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_ThreeQuarterLR;
        break;
    case Tool_HalfAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfAuto;
        break;
    case Tool_HalfBottomStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfBottom;
        break;
    case Tool_HalfTopStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_HalfTop;
        break;
    case Tool_QuarterAutoStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterAuto;
        break;
    case Tool_QuarterULStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterUL;
        break;
    case Tool_QuarterURStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterUR;
        break;
    case Tool_QuarterLLStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterLL;
        break;
    case Tool_QuarterLRStitch:
        m_toolType = ToolType_Stitch;
        m_stitchType = Stitch_QuarterLR;
        break;
    case Tool_SpecialStitch:
        m_toolType = ToolType_Stitch;
        // FIXME
        break;
    case Tool_FrenchKnot:
        m_toolType = ToolType_Knot;
        // FIXME
        break;
    case Tool_LazyDaisyKnot:
        m_toolType = ToolType_Knot;
        // FIXME
        break;
    case Tool_Bead:
        m_toolType = ToolType_Bead;
        break;
    case Tool_Backstitch:
        m_toolType = ToolType_Backstitch;
        break;
    case Tool_Couching:
        m_toolType = ToolType_Couching;
        break;
    case Tool_RectangularSelect:
        m_toolType = ToolType_Select;
        // FIXME
        break;
    case Tool_FreeFormSelect:
        m_toolType = ToolType_Select;
        // FIXME
        break;
    default:
        assert(0);
    }
}

#endif

void XSController::propertiesWindowClosing(XSProperties const& properties)
{
    m_propertiesWindow = 0;
    m_commandStack.doCommand(new XSCommandSetProperties(properties));
}

void XSController::flossPaletteWindowClosing(XSFlossPalette const& flossPalette)
{
    m_editFlossWindow = 0;
    m_commandStack.doCommand(new XSCommandSetFlossPalette(flossPalette));
}

void XSController::onClearSquare(unsigned int x, unsigned int y)
{
    m_commandStack.doCommand(new XSCommandClearSquare(x, y, m_model->getCurrentLayerIndex()));
}

void XSController::onClearSquares(std::vector<XSPoint> const& points)
{
    m_commandStack.doCommand(new XSCommandClearSquares(points, m_model->getCurrentLayerIndex()));
}

void XSController::onSetKnot(unsigned int x, unsigned int y, unsigned int region)
{
    m_commandStack.doCommand(new XSCommandSetKnot(x, y, region, m_model->toolState().m_knotType,
            m_model->toolState().m_flossIndex, m_model->toolState().m_overwrite));
}

void XSController::onSetStitches(std::vector<XSPoint>& points)
{
    m_commandStack.doCommand(new XSCommandSetStitches(points, m_model->toolState().m_stitchTypeContinued,
            m_model->toolState().m_flossIndex, m_model->toolState().m_overwrite));
}

void XSController::onSetStitch(
        unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent)
{
    m_commandStack.doCommand(
            new XSCommandSetStitch(x, y, xPercent, yPercent, m_model->toolState().m_stitchType,
                    m_model->toolState().m_flossIndex, m_model->toolState().m_overwrite));
    m_model->toolState().m_stitchTypeContinued = m_model->toolState().m_stitchType;
}

void XSController::onFloodFill(unsigned int x, unsigned int y, XSSquareIO const& newSquare)
{
    m_commandStack.doCommand(new XSCommandFloodFill(x, y, newSquare));
}

void XSController::onSetBead(unsigned int x, unsigned int y, unsigned int region)
{
    m_commandStack.doCommand(new XSCommandSetBead(
            x, y, region, m_model->toolState().m_beadIndex, m_model->toolState().m_overwrite));
}

StitchType XSController::getStitchType() const
{
    return m_model->m_toolState.m_stitchType;
}

ToolType XSController::getToolType() const
{
    return m_model->m_toolState.m_toolType;
}

#if 0
XSRect XSController::getPreferredFrame(unsigned int x,
        unsigned int y)
{
    unsigned int zoom = XSCanvas::getPreferredZoom();
    XSRect frame(0, 0, x * zoom, y * zoom);

    XSRect prevFrame;
    XSController *document = xs_app->m_manager.GetCurrentDocument();

    if (document)
        prevFrame = document->Frame();
    else
        prevFrame = MTPosOriginFrame();

    frame = MTPosCascadeOff(frame, prevFrame, true);

    return frame;
}

#endif

#if 0
void XSController::onKeyDown(KeyEvent &event)
{
    unsigned int zoom = m_model->getZoom();

    switch (event.GetKeyCode()) {
    case 'A':
    case 'a':
        m_view->scrollBy(zoom, 0);
        break;
    case 'S':
    case 's':
        m_view->scrollBy(0, -zoom);
        break;
    case 'W':
    case 'w':
        m_view->scrollBy(0, zoom);
        break;
    case 'D':
    case 'd':
        m_view->scrollBy(-zoom, 0);
        break;
    case KeyEvent::LEFT:
        m_view->moveCursorBy(-1, 0);
        break;
    case KeyEvent::RIGHT:
        m_view->moveCursorBy(1, 0);
        break;
    case KeyEvent::UP:
        m_view->moveCursorBy(0, -1);
        break;
    case KeyEvent::DOWN:
        m_view->moveCursorBy(0, 1);
        break;
    case ' ':
    {
        // m_view->keyboardClick();
        break;
    }
    case '[':
        m_model->previousFloss();
        break;
    case ']':
        m_model->nextFloss();
        break;
    }
}

#endif

void XSController::setFloss(unsigned int i)
{
    m_model->setFloss(i);
}

void XSController::nextFloss()
{
    m_model->nextFloss();
}

void XSController::previousFloss()
{
    m_model->previousFloss();
}
