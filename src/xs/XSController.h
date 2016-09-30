#ifndef XS_CONTROLLER_H
#define XS_CONTROLLER_H

/** @file
 */

#include "XSCommandStack.h"
#include "XSModel.h"
#include "XSPoint.h"

#include <vector>

class CommandEvent;
class XSEditFlossWindow;
class XSPropertiesWindow;


/**
 */
class XSController {
public:
    XSController(XSModel* model);

    void save(const XSModel& model, const char* filename);
    void open(const char* filename);

    void onClearSquare(unsigned int x, unsigned int y);
    void onClearSquares(std::vector<XSPoint> const& points);
    void onClick(unsigned int x, unsigned int y);
    void onColorPicker(CommandEvent& event);
    void onDesignStyle(CommandEvent& event);
    void onEditFlossPalette(CommandEvent& event);
    void onExport(CommandEvent& event);
    void onFloodFill(unsigned int x, unsigned int y, XSSquareIO const& newsq);
    void onFullStitch();
    void onHalfAutoStitch();
    void onHalfBottomStitch();
    void onHalfTopStitch();
    void onImport(CommandEvent& event);
    // void onKeyDown(KeyEvent& event);
    void onLayerAdd(CommandEvent& event);
    void onLayerDel(CommandEvent& event);
    void onLayerDown(CommandEvent& event);
    void onLayerUp(CommandEvent& event);
    void onOverwrite();
    void onPatternStyle(CommandEvent& event);
    void onProperties(CommandEvent& event);
    void onQuarterAutoStitch();
    void onQuarterLLStitch();
    void onQuarterLRStitch();
    void onQuarterULStitch();
    void onQuarterURStitch();
    void onRealisticStyle(CommandEvent& event);
    void onSetBead(unsigned int x, unsigned int y, unsigned int region);
    void onSetKnot(unsigned int x, unsigned int y, unsigned int region);
    void onSetStitch(unsigned int x, unsigned int y, unsigned int xPercent, unsigned int yPercent);
    void onSetStitches(std::vector<XSPoint>& points);
    void onShowGrid(CommandEvent& event);
    void onThreeQuarterAutoStitch();
    void onThreeQuarterLLStitch();
    void onThreeQuarterLRStitch();
    void onThreeQuarterULStitch();
    void onThreeQuarterURStitch();
    void onToolEraserLarge(CommandEvent& event);
    void onToolEraserMedium(CommandEvent& event);
    void onToolEraserSmall(CommandEvent& event);
    void onToolFloodFill(CommandEvent& event);
    void onZoomIn(CommandEvent& event);
    void onZoomOut(CommandEvent& event);
    void setFloss(unsigned int i);
    void nextFloss();
    void previousFloss();

    void propertiesWindowClosing(XSProperties const& properties);
    void flossPaletteWindowClosing(XSFlossPalette const& flossPalette);

    inline XSModel* getModel()
    {
        return m_model;
    }

    StitchType getStitchType() const;
    ToolType getToolType() const;

private:
    XSCommandStack m_commandStack;

public:
    XSModel* m_model;

private:
    XSPropertiesWindow* m_propertiesWindow;
    XSEditFlossWindow* m_editFlossWindow;
};

#endif
